
#include "Sandbox.hh"

#include "RockHopper/Window/Platform/GLFW_Window.hh"
#include "RockHopper/Rendering/Camera/Camera.hh"
#include "RockHopper/Rendering/Camera/Projection.hh"

static RockHopper::WindowDetails GetInitialWindowDetails()
{
    return RockHopper::WindowDetails
    {
        .title = "RockHopper Client",
        .width = 800, .height = 600,
    };
}

static TimeSpan FPS_120 = 1.0 / 120;

Sandbox::Sandbox()
    : m_Engine{std::make_unique<Engine>("default")}
    , m_Window{std::make_unique<GLFW_Window>(Renderer::API::OpenGL,GetInitialWindowDetails())}
    , m_Keyboard{std::make_unique<Keyboard>("default")}
    , m_Mouse{std::make_unique<Mouse>("default")}
{
    // m_Engine->timing().set_omega(FPS_120);
    m_Engine->insert_event_listener(m_Window.get());
    m_Window->swap_interval(1);

    m_Window->attach(m_Keyboard.get());
    m_Window->attach(m_Mouse.get());

    m_Engine->insert_event_listener(m_Cube.get());
    m_Window->insert_event_listener(m_Cube.get());
}

Sandbox::~Sandbox()
{
    m_Engine->remove_event_listener(m_Cube.get());
    m_Window->remove_event_listener(m_Cube.get());

    m_Window->detach(m_Keyboard.get());
    m_Window->detach(m_Mouse.get());
}

void Sandbox::run()
{
    float w = m_Window->get_details().width, h = m_Window->get_details().height;
    m_Window->camera().projection(std::make_shared<ProjectionPerspective3D>(45,1,100));

    class CameraAnimation : public EngineEvent::ListenerType
    {
    public:
        explicit CameraAnimation(Camera& camera)
            : m_Camera{camera}
        {
            m_Camera.position(glm::vec3{0.0f,0.0f,4.0f});
        }
        virtual void on_event(EngineTickEvent const& event) override
        {
            m_SigmaTime = fmod(m_SigmaTime+(float)event.delta,2*M_PI);
            m_Camera.position(glm::vec3{2.0f*sin(m_SigmaTime),2.0f*cos(m_SigmaTime),6.0f});
        }
    private:
        Camera& m_Camera;
        float m_SigmaTime{};
    };
    CameraAnimation camera_animation {m_Window->camera()};
    m_Engine->insert_event_listener(&camera_animation);

    m_Engine->start();
    m_Engine->stop_notifier().wait();
}

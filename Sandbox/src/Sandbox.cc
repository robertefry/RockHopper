
#include "Sandbox.hh"

#include "RockHopper/Window/Platform/GLFW_Window.hh"
#include "RockHopper/Rendering/Camera.hh"
#include "RockHopper/Rendering/Projection.hh"

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
    m_Engine->timing().set_omega(FPS_120);
    m_Engine->insert_event_listener(m_Window.get());

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

    m_Engine->start();
    m_Engine->stop_notifier().wait();
}

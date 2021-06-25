
#include "Sandbox.hh"

#include "RockHopper/Logging/Logger.hh"
#include "RockHopper/Input/KeyEvents.hh"
#include "RockHopper/Event/EventListener.hh"

#include <GLFW/glfw3.h>

static RockHopper::WindowDetails GetInitialWindowDetails()
{
    return RockHopper::WindowDetails
    {
        .width = 800,
        .height = 600,
        .title = "RockHopper Client",
        .frametime = 1'000'000'000 / 60,
    };
}

Sandbox::Sandbox()
    : m_Window{GetInitialWindowDetails()}
    , m_Keyboard{}
{
    m_Window.attach(&m_Keyboard);
    m_Window.start();
}

Sandbox::~Sandbox()
{
}

void Sandbox::run()
{
    using namespace RockHopper;

    struct BackgroundChanger : KeyEvent::ListenerType
    {
        void on_event(KeyReleaseEvent const& event) override
        {
            if (event.key == KeyCode::KEY_SPACE)
            {
                event.keyboard->window()->insert_task([]()
                {
                    float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
                    float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
                    float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
                    glClearColor(r,g,b,1);
                });
            }
        }
    };
    BackgroundChanger background_changer;
    m_Window.keyboard()->KeyEventHandler::insert_event_listener(&background_changer);

    EventWaitListener<EngineTerminationEvent> termination_event_listener;
    m_Window.Engine::insert_event_listener(&termination_event_listener);
    termination_event_listener.wait();
}

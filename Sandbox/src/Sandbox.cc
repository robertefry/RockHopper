
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

    auto background_changer = EventFunctionListener<KeyEvent,KeyReleaseEvent>([](KeyReleaseEvent const& event)
    {
        if (event.key == KeyCode::KEY_SPACE)
        {
            float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            glClearColor(r,g,b,1);
        }
    });
    m_Window.keyboard()->KeyEventHandler::insert_event_listener(&background_changer);

    EventWaitListener<EngineTerminationEvent> termination_event_listener;
    m_Window.Engine::insert_event_listener(&termination_event_listener);
    termination_event_listener.wait();

    // m_Window will destruct here
}

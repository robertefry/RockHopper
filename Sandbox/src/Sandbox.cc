
#include "Sandbox.hh"

#include "RockHopper/Logging/Logger.hh"
#include "RockHopper/Input/Keyboard/KeyEvents.hh"
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
    using namespace RockHopper;

    m_Keyboard.persist_event_listener(EventFunctionListener<KeyEvent,KeyPressEvent>{[](KeyPressEvent const& event)
    {
        if (event.key == KeyCode::KEY_SPACE)
        {
            float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            glClearColor(r,g,b,1);
        }
    }});
    m_Keyboard.persist_event_listener(EventFunctionListener<KeyEvent,KeyPressEvent>{[](KeyPressEvent const& event)
    {
        if (event.key == KeyCode::KEY_Q)
        {
            ROCKHOPPER_LOG_DEBUG("Querying the state of KEY_W - {}.",(event.keyboard->key(KeyCode::KEY_W).down() ? "down" : "up"));
        }
    }});

    m_Window.attach(&m_Keyboard);
    m_Window.start();
}

Sandbox::~Sandbox()
{
}

void Sandbox::run()
{
    using namespace RockHopper;

    EventWaitListener<EngineTerminationEvent> termination_event_listener;
    m_Window.Engine::insert_event_listener(&termination_event_listener);
    termination_event_listener.wait();

    // m_Window will destruct here
}

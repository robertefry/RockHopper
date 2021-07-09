
#include "Sandbox.hh"

#include "RockHopper/Logging/Logger.hh"
#include "RockHopper/Event/EventListeners.hh"
#include "RockHopper/Input/Keyboard/KeyEvents.hh"
#include "RockHopper/Input/Mouse/MouseEvents.hh"

#include <GLFW/glfw3.h>

static RockHopper::WindowDetails GetInitialWindowDetails()
{
    return RockHopper::WindowDetails
    {
        .title = "RockHopper Client",
        .width = 800, .height = 600,
        .frametime = 1'000'000'000 / 60,
    };
}

Sandbox::Sandbox()
    : m_Window{GetInitialWindowDetails()}
    , m_Keyboard{"default"}
    , m_Mouse{"default"}
    , m_Background{}
{
    m_Window.attach(&m_Keyboard);
    m_Window.attach(&m_Mouse);

    m_Window.insert_event_listener(&m_Background);
    m_Keyboard.insert_event_listener(&m_Background);
    m_Mouse.insert_event_listener(&m_Background);
}

Sandbox::~Sandbox()
{
    m_Window.remove_event_listener(&m_Background);
    m_Keyboard.remove_event_listener(&m_Background);
    m_Mouse.remove_event_listener(&m_Background);

    m_Window.detach(&m_Keyboard);
    m_Window.detach(&m_Mouse);
}

void Sandbox::run()
{
    auto& stop = m_Window.start();
    stop.wait();
}

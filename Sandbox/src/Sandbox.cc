
#include "Sandbox.hh"

#include "RockHopper/Event/EventListener.hh"
#include "RockHopper/Logging/Logger.hh"
#include "RockHopper/Window/Window.hh"

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
{
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
}

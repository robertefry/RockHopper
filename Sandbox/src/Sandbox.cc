
#include "Sandbox.hh"

#include "RockHopper/Logging/Logger.hh"
#include "RockHopper/Window/Window.hh"

Sandbox::Sandbox()
{
}

Sandbox::~Sandbox()
{
}

void Sandbox::run()
{
    using namespace RockHopper;

    WindowDetails window_details
    {
        .width = 800,
        .height = 600,
        .title = "RockHopper Client",
        .frametime = 1'000'000'000 / 60,
    };
    Window window {window_details};

    EventWaitListener<EngineTerminationEvent> termination_event_listener;
    window.EventHandler<EngineTerminationEvent>::insert_event_listener(&termination_event_listener);

    window.start();
    termination_event_listener.wait();
}

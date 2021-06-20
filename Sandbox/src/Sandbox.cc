
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
    RockHopper::WindowDetails window_details
    {
        .width = 800,
        .height = 600,
        .title = "RockHopper Client",
        .frametime = 1'000'000'000 / 60,
    };

    RockHopper::Window window {window_details};
    window.start();
    while (window.alive());
}


#include "RockHopper/UI/Window.hh"

namespace RockHopper::UI
{

    void Window::on_event(Chrono::TickEvent_Init const&)
    {
        dispatch_event(WindowEvent_Init{});
    }

    void Window::on_event(Chrono::TickEvent_Dispose const&)
    {
        dispatch_event(WindowEvent_Dispose{});
    }

    void Window::on_event(Chrono::TickEvent_OnTick const&)
    {
        dispatch_event(WindowEvent_Refresh{});
    }

} // namespace RockHopper::UI

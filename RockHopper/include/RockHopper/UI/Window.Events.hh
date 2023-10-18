
#ifndef ROCKHOPPER_UI_WINDOW_EVENTS_HH
#define ROCKHOPPER_UI_WINDOW_EVENTS_HH

#include "RockHopper/UI/Window.hh"
#include "RockHopper/UI/Window.Events.fwd"

namespace RockHopper::UI
{

    struct WindowEvent_Init
    {
    };

    struct WindowEvent_Dispose
    {
    };

    struct WindowEvent_Refresh
    {
    };

    struct WindowEvent_OnChange_Visible
    {
        Window::visible_t visible;
    };

    struct WindowEvent_OnChange_Focus
    {
        Window::focused_t focus;
    };

    struct WindowEvent_OnChange_Position
    {
        Window::position_t position;
    };

    struct WindowEvent_OnChange_Dimensions
    {
        Window::dimension_t dimensions;
    };

    struct WindowEvent_OnChange_Title
    {
        Window::title_t title;
    };

    struct WindowEvent_OnChange_SwapInterval
    {
        Window::swap_interval_t swap_interval;
    };

} // namespace RockHopper::UI

#endif /* ROCKHOPPER_UI_WINDOW_EVENTS_HH */

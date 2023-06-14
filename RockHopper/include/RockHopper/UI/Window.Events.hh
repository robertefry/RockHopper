
#ifndef ROCKHOPPER_UI_WINDOW_EVENTS_HH
#define ROCKHOPPER_UI_WINDOW_EVENTS_HH

#include "RockHopper/UI/Window.hh"

#include <cstdint>

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

    struct WindowEvent_Close
    {
        bool closed;
    };

    struct WindowEvent_Focus
    {
        bool focus;
    };

    struct WindowEvent_Minimise
    {
        bool minimised;
    };

    struct WindowEvent_Maximise
    {
        bool maximised;
    };

    struct WindowEvent_Move
    {
        uint32_t x, y;
    };

    struct WindowEvent_Size
    {
        uint32_t width, height;
    };

    struct WindowEvent_Scale
    {
        uint32_t scale_x, scale_y;
    };

    struct WindowEvent_BufferSize
    {
        uint32_t width, height;
    };

} // namespace RockHopper::UI

#endif /* ROCKHOPPER_UI_WINDOW_EVENTS_HH */

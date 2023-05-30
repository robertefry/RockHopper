
#ifndef ROCKHOPPER_UI_WINDOW_EVENTS_HH
#define ROCKHOPPER_UI_WINDOW_EVENTS_HH

#include "RockHopper/Event/Event.hh"

#include <cstdint>

namespace RockHopper::UI
{

    struct WindowEvent_Create
    {
    };

    struct WindowEvent_Destroy
    {
    };

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

    using WindowEvent = Event::EventSet<
        WindowEvent_Create,
        WindowEvent_Destroy,
        WindowEvent_Init,
        WindowEvent_Dispose,
        WindowEvent_Refresh,
        WindowEvent_Close,
        WindowEvent_Focus,
        WindowEvent_Minimise,
        WindowEvent_Maximise,
        WindowEvent_Move,
        WindowEvent_Size,
        WindowEvent_Scale,
        WindowEvent_BufferSize >;

} // namespace RockHopper::UI

#endif /* ROCKHOPPER_UI_WINDOW_EVENTS_HH */

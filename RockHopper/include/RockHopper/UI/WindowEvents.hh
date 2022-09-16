
#ifndef __HH_ROCKHOPPER_WINDOW_EVENTS_
#define __HH_ROCKHOPPER_WINDOW_EVENTS_

#include "RockHopper/UI/WindowEvents.fwd"
#include "RockHopper/UI/Window.fwd"

#include "RockHopper/Event/Event.hh"

namespace RockHopper::UI
{

    struct WindowEvent
        : Event::EventCategory<WindowEventSet>
    {
        Window* window{};
    };

    struct WindowEvent_Create
        : Event::EventListenable<WindowEvent,WindowEvent_Create>
    {
    };

    struct WindowEvent_Destroy
        : Event::EventListenable<WindowEvent,WindowEvent_Destroy>
    {
    };

    struct WindowEvent_Init
        : Event::EventListenable<WindowEvent,WindowEvent_Init>
    {
    };

    struct WindowEvent_Dispose
        : Event::EventListenable<WindowEvent,WindowEvent_Dispose>
    {
    };

    struct WindowEvent_Refresh
        : Event::EventListenable<WindowEvent,WindowEvent_Refresh>
    {
    };

    struct WindowEvent_Close
        : Event::EventListenable<WindowEvent,WindowEvent_Close>
    {
    };

    struct WindowEvent_Focus
        : Event::EventListenable<WindowEvent,WindowEvent_Focus>
    {
        int focus{};
    };

    struct WindowEvent_Minimise
        : Event::EventListenable<WindowEvent,WindowEvent_Minimise>
    {
        int minimised{};
    };

    struct WindowEvent_Maximise
        : Event::EventListenable<WindowEvent,WindowEvent_Maximise>
    {
        int maximised{};
    };

    struct WindowEvent_Move
        : Event::EventListenable<WindowEvent,WindowEvent_Move>
    {
        int x{}, y{};
    };

    struct WindowEvent_Size
        : Event::EventListenable<WindowEvent,WindowEvent_Size>
    {
        int width{}, height{};
    };

    struct WindowEvent_Scale
        : Event::EventListenable<WindowEvent,WindowEvent_Scale>
    {
        int scale_x{}, scale_y{};
    };

    struct WindowEvent_BufferSize
        : Event::EventListenable<WindowEvent,WindowEvent_BufferSize>
    {
        int width{}, height{};
    };

} // namespace RockHopper::UI

#endif /* __HH_ROCKHOPPER_WINDOW_EVENTS_ */


#ifndef __HH_ROCKHOPPER_WINDOW_EVENTS_
#define __HH_ROCKHOPPER_WINDOW_EVENTS_

#include "RockHopper/Window/Window.fwd"
#include "RockHopper/Window/WindowEvents.fwd"

#include "RockHopper/Event/Event.hh"
#include "RockHopper/Event/EventHandler.hh"

namespace RockHopper
{

    struct WindowEvent
        : EventCategory<WindowEventSet>
    {
        Window* window{};
    };

    struct WindowPositionEvent : EventListenable<WindowPositionEvent,WindowEvent>
    {
        int x{}, y{};
    };

    struct WindowSizeEvent : EventListenable<WindowSizeEvent,WindowEvent>
    {
        int width{}, height{};
    };

    struct WindowCloseEvent : EventListenable<WindowCloseEvent,WindowEvent>
    {
    };

    struct WindowRefreshEvent : EventListenable<WindowRefreshEvent,WindowEvent>
    {
    };

    struct WindowFocusEvent : EventListenable<WindowFocusEvent,WindowEvent>
    {
        int focus{};
    };

    struct WindowMinimizeEvent : EventListenable<WindowMinimizeEvent,WindowEvent>
    {
        int minimized{};
    };

    struct WindowMaximizedEvent : EventListenable<WindowMaximizedEvent,WindowEvent>
    {
        int maximized{};
    };

    struct WindowScaleEvent : EventListenable<WindowScaleEvent,WindowEvent>
    {
        float scale_x{}, scale_y{};
    };

    struct WindowBufferSizeEvent : EventListenable<WindowBufferSizeEvent,WindowEvent>
    {
        int width{}, height{};
    };

    struct WindowEventHandler
        : EventHandler<WindowEvent>
    {};

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WINDOW_EVENTS_ */

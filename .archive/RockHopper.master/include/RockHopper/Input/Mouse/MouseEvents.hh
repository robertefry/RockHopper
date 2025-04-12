
#ifndef __HH_ROCKHOPPER_INPUT_MOUSE_EVENTS_
#define __HH_ROCKHOPPER_INPUT_MOUSE_EVENTS_

#include "RockHopper/Input/Mouse/Mouse.fwd"
#include "RockHopper/Input/Mouse/MouseEvents.fwd"
#include "RockHopper/Input/Codes.hh"

#include "RockHopper/Event/Event.hh"
#include "RockHopper/Event/EventHandler.hh"

namespace RockHopper
{

    struct MouseEvent
        : EventCategory<MouseEventSet>
    {
        Mouse* mouse{};
    };

    struct MouseMoveEvent : EventListenable<MouseMoveEvent,MouseEvent>
    {
        double x{}, y{};
    };

    struct MouseDragEvent : EventListenable<MouseDragEvent,MouseEvent>
    {
        double x{}, y{};
    };

    struct MouseEnterEvent : EventListenable<MouseEnterEvent,MouseEvent>
    {
    };

    struct MouseExitEvent : EventListenable<MouseExitEvent,MouseEvent>
    {
    };

    struct MousePressEvent : EventListenable<MousePressEvent,MouseEvent>
    {
        MouseCode button{};
        int mods{};
    };

    struct MouseReleaseEvent : EventListenable<MouseReleaseEvent,MouseEvent>
    {
        MouseCode button{};
        int mods{};
    };

    struct MouseRepeatEvent : EventListenable<MouseRepeatEvent,MouseEvent>
    {
        MouseCode button{};
        int mods{};
    };

    struct MouseScrollEvent : EventListenable<MouseScrollEvent,MouseEvent>
    {
        double offset_x{}, offset_y{};
    };

} // namespace RockHopper

namespace std
{

    template <typename C>
    std::basic_ostream<C>& operator<<(std::basic_ostream<C>& ost, RockHopper::MouseMoveEvent const& event)
    {
        ost << "MouseMoveEvent[" << "x=" << event.x << "," << "y=" << event.y << "]";
    }

    template <typename C>
    std::basic_ostream<C>& operator<<(std::basic_ostream<C>& ost, RockHopper::MouseDragEvent const& event)
    {
        ost << "MouseDragEvent[" << "x=" << event.x << "," << "y=" << event.y << "]";
    }

    template <typename C>
    std::basic_ostream<C>& operator<<(std::basic_ostream<C>& ost, RockHopper::MouseEnterEvent const& event)
    {
        ost << "MouseEnterEvent[" << "]";
    }

    template <typename C>
    std::basic_ostream<C>& operator<<(std::basic_ostream<C>& ost, RockHopper::MouseExitEvent const& event)
    {
        ost << "MouseExitEvent[" << "]";
    }

    template <typename C>
    std::basic_ostream<C>& operator<<(std::basic_ostream<C>& ost, RockHopper::MousePressEvent const& event)
    {
        ost << "MousePressEvent[" << "button=" << event.button << "]";
    }

    template <typename C>
    std::basic_ostream<C>& operator<<(std::basic_ostream<C>& ost, RockHopper::MouseReleaseEvent const& event)
    {
        ost << "MouseReleaseEvent[" << "button=" << event.button << "]";
    }

    template <typename C>
    std::basic_ostream<C>& operator<<(std::basic_ostream<C>& ost, RockHopper::MouseRepeatEvent const& event)
    {
        ost << "MouseRepeatEvent[" << "button=" << event.button << "]";
    }

    template <typename C>
    std::basic_ostream<C>& operator<<(std::basic_ostream<C>& ost, RockHopper::MouseScrollEvent const& event)
    {
        ost << "MouseScrollEvent[" << "x=" << event.offset_x << "," << "y=" << event.offset_y << "]";
    }

} // namespace std

#endif /* __HH_ROCKHOPPER_INPUT_MOUSE_EVENTS_ */

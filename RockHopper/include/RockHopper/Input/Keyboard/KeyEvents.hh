
#ifndef __HH_ROCKHOPPER_INPUT_KEYBOARD_EVENTS_
#define __HH_ROCKHOPPER_INPUT_KEYBOARD_EVENTS_

#include "RockHopper/Input/Keyboard/Keyboard.fwd"
#include "RockHopper/Input/Keyboard/KeyEvents.fwd"
#include "RockHopper/Input/Codes.hh"

#include "RockHopper/Event/Event.hh"
#include "RockHopper/Event/EventHandler.hh"

#include <iosfwd>

namespace RockHopper
{

    struct KeyEvent
        : I_EventListenable<KeyEventSet>
    {
        Keyboard* keyboard;
    };

    struct KeyPressEvent : EventListenable<KeyPressEvent,KeyEvent>
    {
        KeyCode key;
        int mods;
        int scancode;
    };

    struct KeyReleaseEvent : EventListenable<KeyReleaseEvent,KeyEvent>
    {
        KeyCode key;
        int mods;
        int scancode;
    };

    struct KeyRepeatEvent : EventListenable<KeyRepeatEvent,KeyEvent>
    {
        KeyCode key;
        int mods;
        int scancode;
    };

    struct KeyEventHandler
        : EventHandler<KeyEvent>
    {};

} // namespace RockHopper

namespace std
{

    template <typename C>
    std::basic_ostream<C>& operator<<(std::basic_ostream<C>& ost, RockHopper::KeyPressEvent const& event)
    {
        return ost << "KeyPressEvent[" << event.key << "]";
    }

    template <typename C>
    std::basic_ostream<C>& operator<<(std::basic_ostream<C>& ost, RockHopper::KeyReleaseEvent const& event)
    {
        return ost << "KeyReleaseEvent[" << event.key << "]";
    }

    template <typename C>
    std::basic_ostream<C>& operator<<(std::basic_ostream<C>& ost, RockHopper::KeyRepeatEvent const& event)
    {
        return ost << "KeyRepeatEvent[" << event.key << "]";
    }

} // namespace std

#endif /* __HH_ROCKHOPPER_INPUT_KEYBOARD_EVENTS_ */

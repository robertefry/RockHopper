
#ifndef __HH_ROCKHOPPER_INPUT_KEYBOARD_EVENTS_
#define __HH_ROCKHOPPER_INPUT_KEYBOARD_EVENTS_

#include "RockHopper/Input/Keyboard.fwd"
#include "RockHopper/Input/KeyEvents.fwd"
#include "RockHopper/Input/Codes.hh"

#include "RockHopper/Event/Event.hh"
#include "RockHopper/Event/EventHandler.hh"

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

#endif /* __HH_ROCKHOPPER_INPUT_KEYBOARD_EVENTS_ */

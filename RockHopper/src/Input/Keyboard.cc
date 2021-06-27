
#include "RockHopper/Input/Keyboard/Keyboard.hh"
#include "RockHopper/Event/EventListener.hh"

#include "RockHopper/Window/Window.hh"

namespace RockHopper
{

    Keyboard::~Keyboard()
    {
        if (m_WindowHandle) m_WindowHandle->detach(this);
    }

    Keyboard::Keyboard()
    {
        for (int i = 0; i < (int)KeyCode::KEY_LAST; ++i)
        {
            KeyCode keycode = static_cast<KeyCode>(i);
            m_KeyMap.emplace(keycode,Key{i});
        }

        persist_event_listener(EventFunctionListener<KeyEvent,KeyPressEvent>{[](KeyPressEvent const& event)
        {
            event.keyboard->key(event.key).press();
        }});
        persist_event_listener(EventFunctionListener<KeyEvent,KeyReleaseEvent>{[](KeyReleaseEvent const& event)
        {
            event.keyboard->key(event.key).release();
        }});
        persist_event_listener(EventFunctionListener<KeyEvent,KeyRepeatEvent>{[](KeyRepeatEvent const& event)
        {
            event.keyboard->key(event.key).repeat();
        }});
    }

    Keyboard::Keyboard(Keyboard&& other)
    {
        *this = std::move(other);
    }

    Keyboard& Keyboard::operator=(Keyboard&& other)
    {
        m_WindowHandle = other.m_WindowHandle;
        other.m_WindowHandle = nullptr;

        if (m_WindowHandle)
        {
            m_WindowHandle->detach(&other);
            m_WindowHandle->attach(this);
        }
        return *this;
    }

} // namespace RockHopper

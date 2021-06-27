
#include "RockHopper/Input/Mouse/Mouse.hh"
#include "RockHopper/Event/EventListener.hh"

#include "RockHopper/Window/Window.hh"

namespace RockHopper
{

    Mouse::~Mouse()
    {
        if (m_WindowHandle) m_WindowHandle->detach(this);
    }

    Mouse::Mouse(Mouse&& other)
    {
        *this = std::move(other);
    }

    Mouse& Mouse::operator=(Mouse&& other)
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

    Mouse::Mouse()
    {
        for (int i = 0; i < (int)MouseCode::BUTTON_LAST; ++i)
        {
            MouseCode keycode = static_cast<MouseCode>(i);
            m_KeyMap.emplace(keycode,Key{i});
        }

        persist_event_listener(EventFunctionListener<MouseEvent,MouseMoveEvent>{[](MouseMoveEvent const& event)
        {
            event.mouse->set_position(event.x,event.y);
        }});
        persist_event_listener(EventFunctionListener<MouseEvent,MouseDragEvent>{[](MouseDragEvent const& event)
        {
            event.mouse->set_position(event.x,event.y);
        }});
        persist_event_listener(EventFunctionListener<MouseEvent,MouseEnterEvent>{[](MouseEnterEvent const& event)
        {
            event.mouse->set_in_window(true);
        }});
        persist_event_listener(EventFunctionListener<MouseEvent,MouseExitEvent>{[](MouseExitEvent const& event)
        {
            event.mouse->set_in_window(false);
        }});
        persist_event_listener(EventFunctionListener<MouseEvent,MousePressEvent>{[](MousePressEvent const& event)
        {
            event.mouse->key(event.button).press();
        }});
        persist_event_listener(EventFunctionListener<MouseEvent,MouseReleaseEvent>{[](MouseReleaseEvent const& event)
        {
            event.mouse->key(event.button).release();
        }});
        persist_event_listener(EventFunctionListener<MouseEvent,MouseRepeatEvent>{[](MouseRepeatEvent const& event)
        {
            event.mouse->key(event.button).repeat();
        }});
    }

} // namespace RockHopper


#include "RockHopper/Input/Mouse/Mouse.hh"
#include "RockHopper/Event/EventListeners.hh"

#include "RockHopper/Window/Window.hh"

namespace RockHopper
{

    Mouse::~Mouse()
    {
        if (m_WindowHandle) m_WindowHandle->detach(this);
    }

    Mouse::Mouse(std::string const& name)
        : m_DebugName{"Mouse",name}
    {
        set_parallel_dispatch(true);

        for (int i = 0; i < (int)MouseCode::BUTTON_LAST; ++i)
        {
            MouseCode keycode = static_cast<MouseCode>(i);
            m_KeyMap.emplace(keycode,Key{i});
        }

        give_event_listener(EventFunctionListener<MouseMoveEvent>{[](MouseMoveEvent const& event)
        {
            event.mouse->set_position(event.x,event.y);
        }});
        give_event_listener(EventFunctionListener<MouseDragEvent>{[](MouseDragEvent const& event)
        {
            event.mouse->set_position(event.x,event.y);
        }});
        give_event_listener(EventFunctionListener<MouseEnterEvent>{[](MouseEnterEvent const& event)
        {
            event.mouse->set_in_window(true);
        }});
        give_event_listener(EventFunctionListener<MouseExitEvent>{[](MouseExitEvent const& event)
        {
            event.mouse->set_in_window(false);
        }});
        give_event_listener(EventFunctionListener<MousePressEvent>{[](MousePressEvent const& event)
        {
            event.mouse->key(event.button).press();
        }});
        give_event_listener(EventFunctionListener<MouseReleaseEvent>{[](MouseReleaseEvent const& event)
        {
            event.mouse->key(event.button).release();
        }});
        give_event_listener(EventFunctionListener<MouseRepeatEvent>{[](MouseRepeatEvent const& event)
        {
            event.mouse->key(event.button).repeat();
        }});
    }

    Mouse::Mouse(Mouse&& other)
        : m_DebugName{other.m_DebugName}
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

    void Mouse::on_attach(Window* window)
    {
        if (m_WindowHandle != nullptr)
        {
            ROCKHOPPER_INTERNAL_LOG_ERROR("{} already attached to {}", m_WindowHandle->m_DebugName, m_DebugName);
            return;
        }
        m_WindowHandle = window;
    }

    void Mouse::on_detach(Window* window)
    {
        m_WindowHandle = nullptr;
    }

} // namespace RockHopper

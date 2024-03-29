
#include "RockHopper/Input/Keyboard/Keyboard.hh"
#include "RockHopper/Event/EventListeners.hh"

#include "RockHopper/Window/Window.hh"

namespace RockHopper
{

    Keyboard::~Keyboard()
    {
        if (m_WindowHandle) m_WindowHandle->detach(this);
    }

    Keyboard::Keyboard(std::string const& name)
        : m_DebugName{"Keyboard",name}
    {
        set_parallel_dispatch(true);

        for (int i = 0; i < (int)KeyCode::KEY_LAST; ++i)
        {
            KeyCode keycode = static_cast<KeyCode>(i);
            m_KeyMap.emplace(keycode,Key{i});
        }

        give_event_listener(EventFunctionListener<KeyPressEvent>{[](KeyPressEvent const& event)
        {
            event.keyboard->key(event.key).press();
        }});
        give_event_listener(EventFunctionListener<KeyReleaseEvent>{[](KeyReleaseEvent const& event)
        {
            event.keyboard->key(event.key).release();
        }});
        give_event_listener(EventFunctionListener<KeyRepeatEvent>{[](KeyRepeatEvent const& event)
        {
            event.keyboard->key(event.key).repeat();
        }});
    }

    Keyboard::Keyboard(Keyboard&& other)
        : m_DebugName{other.m_DebugName}
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

    void Keyboard::on_attach(Window* window)
    {
        if (m_WindowHandle != nullptr)
        {
            ROCKHOPPER_INTERNAL_LOG_ERROR("{} already attached to {}", m_WindowHandle->m_DebugName, m_DebugName);
            return;
        }
        m_WindowHandle = window;
    }

    void Keyboard::on_detach(Window* window)
    {
        m_WindowHandle = nullptr;
    }

} // namespace RockHopper

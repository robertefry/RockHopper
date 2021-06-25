
#include "RockHopper/Input/Keyboard/Keyboard.hh"

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
    }

    auto Keyboard::window() const -> Window const*
    {
        return m_WindowHandle;
    }

    auto Keyboard::window() -> Window*
    {
        return m_WindowHandle;
    }

} // namespace RockHopper

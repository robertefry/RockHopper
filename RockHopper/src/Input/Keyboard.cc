
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
        for (size_t keycode = 0; keycode < (size_t)KeyCode::KEY_LAST; ++keycode)
        {
            m_KeyMap.emplace(keycode,Key{keycode});
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

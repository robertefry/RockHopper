
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
        for (size_t i = 0; i < (size_t)KeyCode::KEY_LAST; ++i)
        {
            KeyCode keycode = static_cast<KeyCode>(i);
            m_KeyMap.emplace(keycode,Key{keycode});
        }
    }

    auto Keyboard::key(KeyCode keycode) const -> Key const&
    {
        return m_KeyMap.at(keycode);
    }

    auto Keyboard::window() const -> Window const*
    {
        return m_WindowHandle;
    }

    auto Keyboard::window() -> Window*
    {
        return m_WindowHandle;
    }

    void Keyboard::tick()
    {
        for (auto& [code,key] : m_KeyMap)
        {
            key.tick();
        }
    }

} // namespace RockHopper

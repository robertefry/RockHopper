
#include "RockHopper/Input/Keyboard/Key.hh"

#include <GLFW/glfw3.h>

namespace RockHopper
{

    Key::Key(KeyCode keycode)
        : m_KeyCode{keycode}
    {}

    auto Key::code() const -> KeyCode
    {
        return m_KeyCode;
    }

    auto Key::name() const -> std::string
    {
        char const* name = glfwGetKeyName((int)code(),(int)scan());
        return std::string(name);
    }

    auto Key::scan() const -> int
    {
        return glfwGetKeyScancode((int)m_KeyCode);
    }

    void Key::tick()
    {
        m_IsJustDown = false;
        m_IsJustUp = false;
    }

    void Key::press()
    {
        m_IsDown = true;
        m_IsRepeat = false;
        m_IsJustDown = true;
    }

    void Key::release()
    {
        m_IsDown = false;
        m_IsRepeat = false;
        m_IsJustUp = true;
    }

    void Key::repeat()
    {
        m_IsRepeat = true;
        m_IsJustDown = true;
    }

} // namespace RockHopper


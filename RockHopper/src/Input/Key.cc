
#include "RockHopper/Input/Key/Key.hh"

#include <GLFW/glfw3.h>

namespace RockHopper
{

    Key::Key(int enumcode)
        : m_EnumCode{enumcode}
    {}

    Key::Key(Key const& other)
        : m_EnumCode{other.m_EnumCode}
        , m_IsDown{other.down()}
        , m_IsRepeat{other.repeat()}
        , m_IsJustDown{other.just_down()}
        , m_IsJustUp{other.just_up()}
    {
    }

    auto Key::code() const -> int
    {
        return m_EnumCode;
    }

    auto Key::name() const -> std::string
    {
        char const* name = glfwGetKeyName(code(),scan());
        return std::string(name);
    }

    auto Key::scan() const -> int
    {
        return glfwGetKeyScancode(m_EnumCode);
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


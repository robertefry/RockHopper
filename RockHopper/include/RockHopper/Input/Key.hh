
#ifndef __HH_ROCKHOPPER_INPUT_KEY_
#define __HH_ROCKHOPPER_INPUT_KEY_

#include "RockHopper/Input/Codes.hh"
#include "RockHopper/Input/Keyboard.fwd"

#include <string>
#include <iosfwd>

namespace RockHopper
{

    class Key
    {
        friend Keyboard;
    public:
        ~Key() = default;
        explicit Key(KeyCode keycode);

        auto code() const -> KeyCode;
        auto name() const -> std::string;
        auto scan() const -> int;

        inline bool is_down() const { return m_IsDown; }
        inline bool is_repeat() const { return m_IsRepeat; }
        inline bool is_just_down() const { return m_IsJustDown; }
        inline bool is_just_up() const { return m_IsJustUp; }

    protected:
        void tick();
        void press();
        void release();
        void repeat();

    private:
        KeyCode const m_KeyCode;
        bool m_IsDown{}, m_IsRepeat{};
        bool m_IsJustDown{}, m_IsJustUp{};
    };

} // namespace RockHopper

namespace std
{

    template <typename C = char>
    std::basic_ostream<C>& operator<<(std::basic_ostream<C>& ost, RockHopper::Key const& key)
    {
        return ost << "Key["
            << "code=" << key.code() << ","
            << "name=" << key.name() << ","
            << "scan=" << key.scan() << "]";
    }

} // namespace std

#endif /* __HH_ROCKHOPPER_INPUT_KEY_ */

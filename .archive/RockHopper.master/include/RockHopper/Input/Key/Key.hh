
#ifndef __HH_ROCKHOPPER_INPUT_KEY_
#define __HH_ROCKHOPPER_INPUT_KEY_

#include "RockHopper/Input/Codes.hh"

#include <atomic>
#include <string>
#include <iosfwd>

namespace RockHopper
{

    class Key
    {
    public:
        ~Key() = default;
        explicit Key(int enumcode);

        Key(Key const&);

        auto code() const -> int;
        auto name() const -> std::string;
        auto scan() const -> int;

        inline bool down() const { return m_IsDown; }
        inline bool repeat() const { return m_IsRepeat; }
        inline bool just_down() const { return m_IsJustDown; }
        inline bool just_up() const { return m_IsJustUp; }

        void tick();
        void press();
        void release();
        void repeat();

    private:
        int const m_EnumCode;
        std::atomic<bool> m_IsDown{}, m_IsRepeat{};
        std::atomic<bool> m_IsJustDown{}, m_IsJustUp{};
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


#ifndef __HH_ROCKHOPPER_INPUT_KEY_MANAGER_
#define __HH_ROCKHOPPER_INPUT_KEY_MANAGER_

#include "RockHopper/Input/Key/Key.hh"

#include <unordered_map>

/* ************************************************************************** */
// [Definition] RockHopper::KeyManager
/* ************************************************************************** */

namespace RockHopper
{

    template <typename T_EnumCode>
    class KeyManager
    {
    public:
        virtual ~KeyManager() = default;

        inline auto size() const { return m_KeyMap.size(); }

        inline Key& operator[](T_EnumCode enumcode) { return m_KeyMap.at(enumcode); }
        inline Key& key(T_EnumCode enumcode) { return m_KeyMap.at(enumcode); }

        inline Key const& operator[](T_EnumCode enumcode) const { return m_KeyMap.at(enumcode); }
        inline Key const& key(T_EnumCode enumcode) const { return m_KeyMap.at(enumcode); }

        inline auto begin() { return m_KeyMap.begin(); }
        inline auto begin() const { return m_KeyMap.begin(); }
        inline auto end() { return m_KeyMap.end(); }
        inline auto end() const { return m_KeyMap.end(); }

        inline void tick();

    protected:
        std::unordered_map<T_EnumCode,Key> m_KeyMap{};
    };

} // namespace RockHopper

/* ************************************************************************** */
// [Implementation] RockHopper::KeyManager
/* ************************************************************************** */

namespace RockHopper
{

    template <typename T_EnumCode>
    void KeyManager<T_EnumCode>::tick()
    {
        for (auto& [keycode,key] : m_KeyMap)
        {
            key.tick();
        }
    }

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_INPUT_KEY_MANAGER_ */

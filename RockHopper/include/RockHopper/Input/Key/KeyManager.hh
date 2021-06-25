
#ifndef __HH_ROCKHOPPER_INPUT_KEY_MANAGER_
#define __HH_ROCKHOPPER_INPUT_KEY_MANAGER_

#include "RockHopper/Input/Key/Key.hh"

#include <unordered_map>

/* ************************************************************************** */
// [Definition] RockHopper::KeyManager
/* ************************************************************************** */

namespace RockHopper
{

    class KeyManager
    {
    public:
        virtual ~KeyManager() = default;

        inline auto size() const { return m_KeyMap.size(); }
        inline auto operator[](int enumcode) { return m_KeyMap.at(enumcode); }
        inline auto operator[](int enumcode) const { return m_KeyMap.at(enumcode); }

        inline auto begin() { return m_KeyMap.begin(); }
        inline auto begin() const { return m_KeyMap.begin(); }
        inline auto end() { return m_KeyMap.end(); }
        inline auto end() const { return m_KeyMap.end(); }

        inline void tick();

    protected:
        std::unordered_map<int,Key> m_KeyMap{};
    };

} // namespace RockHopper

/* ************************************************************************** */
// [Implementation] RockHopper::KeyManager
/* ************************************************************************** */

namespace RockHopper
{

    void KeyManager::tick()
    {
        for (auto& [keycode,key] : m_KeyMap)
        {
            key.tick();
        }
    }

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_INPUT_KEY_MANAGER_ */

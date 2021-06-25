
#ifndef __HH_ROCKHOPPER_INPUT_KEYBOARD_
#define __HH_ROCKHOPPER_INPUT_KEYBOARD_

#include "RockHopper/Input/Key.hh"
#include "RockHopper/Input/KeyEvents.hh"
#include "RockHopper/Window/Window.fwd"

#include <unordered_map>

namespace RockHopper
{

    class Keyboard
        : public KeyEventHandler
    {
        friend Window;
    public:
        virtual ~Keyboard();
        explicit Keyboard();

        auto key(KeyCode keycode) const -> Key const&;

        auto window() const -> Window const*;
        auto window() -> Window*;

    protected:
        void tick();

    private:
        Window* m_WindowHandle{};
        std::unordered_map<KeyCode,Key> m_KeyMap{};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_INPUT_KEYBOARD_ */

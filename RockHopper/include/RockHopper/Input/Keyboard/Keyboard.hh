
#ifndef __HH_ROCKHOPPER_INPUT_KEYBOARD_
#define __HH_ROCKHOPPER_INPUT_KEYBOARD_

#include "RockHopper/Input/Key/KeyManager.hh"
#include "RockHopper/Input/Keyboard/KeyEvents.hh"
#include "RockHopper/Window/Window.fwd"

namespace RockHopper
{

    class Keyboard
        : public KeyManager<KeyCode>
        , public KeyEventHandler
    {
        friend Window;
    public:
        virtual ~Keyboard();
        explicit Keyboard();

        auto window() const -> Window const*;
        auto window() -> Window*;

    private:
        Window* m_WindowHandle{};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_INPUT_KEYBOARD_ */

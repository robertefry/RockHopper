
#ifndef __HH_ROCKHOPPER_INPUT_KEYBOARD_
#define __HH_ROCKHOPPER_INPUT_KEYBOARD_

#include "RockHopper/Debug.hh"
#include "RockHopper/Input/Key/KeyManager.hh"
#include "RockHopper/Input/Keyboard/KeyEvents.hh"
#include "RockHopper/Window/Window.fwd"

#include <string>

namespace RockHopper
{

    class Keyboard
        : public KeyManager<KeyCode>
        , public KeyEventHandler
    {
    public:
        virtual ~Keyboard();
        explicit Keyboard(std::string const& name);

        explicit Keyboard(Keyboard const&) = delete;
        Keyboard& operator=(Keyboard const&) = delete;

        explicit Keyboard(Keyboard&&);
        Keyboard& operator=(Keyboard&&);

        void on_attach(Window*);
        void on_detach(Window*);

        inline auto window() -> Window* { return m_WindowHandle; }
        inline auto window() const -> Window const* { return m_WindowHandle; }

    public:
        DebugName m_DebugName;

    private:
        Window* m_WindowHandle{};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_INPUT_KEYBOARD_ */

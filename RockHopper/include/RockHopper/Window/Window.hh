
#ifndef __HH_ROCKHOPPER_WINDOW_
#define __HH_ROCKHOPPER_WINDOW_

#include "RockHopper/Window/WindowEvents.hh"
#include "RockHopper/Engine/EngineEvent.hh"

#include "RockHopper/Debug.hh"
#include "RockHopper/Event/EventHandler.hh"
#include "RockHopper/Input/Keyboard/Keyboard.fwd"
#include "RockHopper/Input/Mouse/Mouse.fwd"

namespace RockHopper
{

    struct WindowDetails
    {
        std::string title;
        int width, height;
    };

    class Window
        : public EngineEvent::ListenerType
        , public EventHandler<WindowEvent>
    {
    public:
        virtual ~Window() = default;
        explicit Window(std::string const& debug_name);

        explicit Window(Window const&) = delete;
        Window& operator=(Window const&) = delete;

        virtual void set_details(WindowDetails const&) = 0;
        virtual auto get_details() const -> WindowDetails const& = 0;

        inline auto keyboard() -> Keyboard* { return m_KeyboardHandle; }
        inline auto keyboard() const -> Keyboard const* { return m_KeyboardHandle; }
        virtual void attach(Keyboard*) = 0;
        virtual void detach(Keyboard*) = 0;

        inline auto mouse() -> Mouse* { return m_MouseHandle; }
        inline auto mouse() const -> Mouse const* { return m_MouseHandle; }
        virtual void attach(Mouse*) = 0;
        virtual void detach(Mouse*) = 0;

    public:
        DebugName m_DebugName;

    protected:
        Keyboard* m_KeyboardHandle{};
        Mouse* m_MouseHandle{};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WINDOW_ */

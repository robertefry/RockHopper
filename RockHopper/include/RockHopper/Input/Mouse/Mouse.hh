
#ifndef __HH_ROCKHOPPER_INPUT_MOUSE_
#define __HH_ROCKHOPPER_INPUT_MOUSE_

#include "RockHopper/Debug.hh"
#include "RockHopper/Input/Key/KeyManager.hh"
#include "RockHopper/Input/Mouse/MouseEvents.hh"
#include "RockHopper/Window/Window.fwd"

#include <string>
#include <atomic>
#include <array>

namespace RockHopper
{

    class Mouse
        : public KeyManager<MouseCode>
        , public EventHandler<MouseEvent>
    {
    public:
        virtual ~Mouse();
        explicit Mouse(std::string const& name);

        explicit Mouse(Mouse const&) = delete;
        Mouse& operator=(Mouse const&) = delete;

        explicit Mouse(Mouse&&);
        Mouse& operator=(Mouse&&);

        void on_attach(Window*);
        void on_detach(Window*);

        inline void set_position(double x, double y) { m_PosX = x; m_PosY = y; }
        inline auto get_position() const { return std::array<double,2>{m_PosX,m_PosY}; }

        inline void set_in_window(bool in_window) { m_IsInWindow = in_window; }
        inline bool get_in_window() const { return m_IsInWindow; }

        inline auto window() -> Window* { return m_WindowHandle; }
        inline auto window() const -> Window const* { return m_WindowHandle; }

    public:
        DebugName m_DebugName;

    private:
        std::atomic<double> m_PosX{}, m_PosY{};
        std::atomic<bool> m_IsInWindow{};
        Window* m_WindowHandle{};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_INPUT_MOUSE_ */

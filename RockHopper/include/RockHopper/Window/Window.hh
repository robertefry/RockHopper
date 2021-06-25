
#ifndef __HH_ROCKHOPPER_WINDOW_
#define __HH_ROCKHOPPER_WINDOW_

#include "RockHopper/Engine/Engine.hh"
#include "RockHopper/Window/WindowEvents.hh"
#include "RockHopper/Input/Keyboard/Keyboard.fwd"
#include "RockHopper/Input/Mouse/Mouse.fwd"

class GLFWwindow;

namespace RockHopper
{

    struct WindowDetails
    {
        int width, height;
        std::string title;
        int64_t frametime;
    };

    class Window
        : public RockHopper::Engine
        , public WindowEventHandler
    {
    public:
        explicit Window(WindowDetails const&);
        virtual ~Window();

        explicit Window(Window const&) = delete;
        Window& operator=(Window const&) = delete;

        explicit Window(Window&&);
        Window& operator=(Window&&);

        void set_details(WindowDetails const&);
        auto get_details() const -> WindowDetails const&;

        inline auto keyboard() -> Keyboard* { return m_KeyboardHandle; }
        inline auto keyboard() const -> Keyboard const* { return m_KeyboardHandle; }
        void attach(Keyboard*);
        void detach(Keyboard*);

        inline auto mouse() -> Mouse* { return m_MouseHandle; }
        inline auto mouse() const -> Mouse const* { return m_MouseHandle; }
        void attach(Mouse*);
        void detach(Mouse*);

    protected:
        virtual void init() override;
        virtual void tick() override;
        virtual void dispose() override;

    private:
        GLFWwindow* m_WindowHandle;
        WindowDetails m_Details;

        Keyboard* m_KeyboardHandle{};
        Mouse* m_MouseHandle{};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WINDOW_ */

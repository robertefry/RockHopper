
#ifndef __HH_ROCKHOPPER_WINDOW_
#define __HH_ROCKHOPPER_WINDOW_

#include "RockHopper/Window/WindowEvents.hh"
#include "RockHopper/Window/Backend/GraphicsThread.hh"
#include "RockHopper/Window/Backend/WindowContext.hh"

#include "RockHopper/Engine/Engine.hh"
#include "RockHopper/Input/Keyboard/Keyboard.fwd"
#include "RockHopper/Input/Mouse/Mouse.fwd"

#include <mutex>

class GLFWwindow;

namespace RockHopper
{

    struct WindowDetails
    {
        std::string title;
        int width, height;
        int64_t frametime;
    };

    class Window
        : public RockHopper::Engine
        , public WindowEventHandler
    {
    public:
        virtual ~Window();
        explicit Window(WindowDetails const&);

        explicit Window(Window const&) = delete;
        Window& operator=(Window const&) = delete;

        explicit Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

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
        GraphicsThread m_GraphicsThread;
        WindowContext m_WindowContext;

        GLFWwindow* m_WindowHandle;
        WindowDetails m_Details;
        mutable std::mutex m_WindowMutex{};

        Keyboard* m_KeyboardHandle{};
        Mouse* m_MouseHandle{};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WINDOW_ */

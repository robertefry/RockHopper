
#ifndef __HH_ROCKHOPPER_WINDOW_
#define __HH_ROCKHOPPER_WINDOW_

#include "RockHopper/Engine/Engine.hh"
#include "RockHopper/Window/WindowEvents.hh"
#include "RockHopper/Input/Keyboard.fwd"

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

        auto keyboard() const -> Keyboard const*;
        auto keyboard() -> Keyboard*;
        void attach(Keyboard*);
        void detach(Keyboard*);

    protected:
        virtual void init() override;
        virtual void tick() override;
        virtual void dispose() override;

    private:
        GLFWwindow* m_WindowHandle;
        WindowDetails m_Details;

        Keyboard* m_KeyboardHandle{};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WINDOW_ */

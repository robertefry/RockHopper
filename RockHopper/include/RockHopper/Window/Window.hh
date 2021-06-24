
#ifndef __HH_ROCKHOPPER_WINDOW_
#define __HH_ROCKHOPPER_WINDOW_

#include "RockHopper/Engine/Engine.hh"
#include "RockHopper/Window/WindowEvents.hh"

#include <GLFW/glfw3.h>

#include <string>

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

    protected:
        virtual void init() override;
        virtual void tick() override;
        virtual void dispose() override;

    private:
        GLFWwindow* m_WindowHandle;
        WindowDetails m_Details;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WINDOW_ */

/**
 * @author Robert Fry
 * @date create 20-Jun-2021
 * @date modify 20-Jun-2021
 */

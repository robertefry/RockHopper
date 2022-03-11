
#ifndef __HH_ROCKHOPPER_WINDOW_GLFW_
#define __HH_ROCKHOPPER_WINDOW_GLFW_

#include "RockHopper/Window/Window.hh"
#include "RockHopper/Window/Platform/GLFW_Context.hh"

#include "RockHopper/Utility/Async/TaskQueue.hh"

class GLFWwindow;

namespace RockHopper
{

    class GLFW_Window : public Window
    {
    public:
        virtual ~GLFW_Window();
        explicit GLFW_Window(Renderer::API renderer_api, WindowDetails const&);

        explicit GLFW_Window(GLFW_Window const&) = delete;
        GLFW_Window& operator=(GLFW_Window const&) = delete;

        virtual void set_details(WindowDetails const&) override;
        virtual auto get_details() const -> WindowDetails const& override;

        virtual void swap_interval(int) override;

        virtual void attach(Keyboard*) override;
        virtual void detach(Keyboard*) override;

        virtual void attach(Mouse*) override;
        virtual void detach(Mouse*) override;

    protected:
        virtual void on_event(EngineInitEvent const&) override;
        virtual void on_event(EngineDisposeEvent const&) override;
        virtual void on_event(EngineTickEvent const&) override;

    private:
        GLFWwindow* m_WindowHandle{};
        GLFW_Context m_GlfwContext{};

        TaskQueue m_TaskQueue{};
        WindowDetails m_Details;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WINDOW_GLFW_ */

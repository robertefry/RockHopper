
#ifndef __HH_ROCKHOPPER_WINDOW_GLFW_
#define __HH_ROCKHOPPER_WINDOW_GLFW_

#include "RockHopper/Window/Window.hh"
#include "RockHopper/Window/Platform/GlfwContext.hh"
#include "RockHopper/Rendering/Renderer.hh"

#include "RockHopper/Utility/TaskQueue.hh"

class GLFWwindow;

namespace RockHopper
{

    class GlfwWindow : public Window
    {
    public:
        virtual ~GlfwWindow();
        explicit GlfwWindow(Renderer::API render_api, WindowDetails const&);

        explicit GlfwWindow(GlfwWindow const&) = delete;
        GlfwWindow& operator=(GlfwWindow const&) = delete;

        virtual void set_details(WindowDetails const&) override;
        virtual auto get_details() const -> WindowDetails const& override;

        virtual void attach(Keyboard*) override;
        virtual void detach(Keyboard*) override;

        virtual void attach(Mouse*) override;
        virtual void detach(Mouse*) override;

    protected:
        virtual void on_event(EngineInitEvent const&) override;
        virtual void on_event(EngineTickEvent const&) override;
        virtual void on_event(EngineDisposeEvent const&) override;

    private:
        GLFWwindow* m_WindowHandle{};
        GlfwContext m_GlfwContext{};

        RenderThread m_RenderThread{};
        TaskQueue m_TaskQueue{};
        WindowDetails m_Details;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WINDOW_GLFW_ */

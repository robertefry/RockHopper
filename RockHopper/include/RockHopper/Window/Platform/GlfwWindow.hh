
#ifndef __HH_ROCKHOPPER_WINDOW_GLFW_
#define __HH_ROCKHOPPER_WINDOW_GLFW_

#include "RockHopper/Window/Window.hh"
#include "RockHopper/Window/Platform/GlfwContext.hh"

#include "RockHopper/Rendering/GraphicsThread.hh"
#include "RockHopper/Rendering/RenderContext.hh"

class GLFWwindow;

namespace RockHopper
{

    class GlfwWindow
        : public Window
    {
    public:
        virtual ~GlfwWindow();
        explicit GlfwWindow(RenderContext const&, WindowDetails const&);

        explicit GlfwWindow(GlfwWindow const&) = delete;
        GlfwWindow& operator=(GlfwWindow const&) = delete;

        virtual void set_details(WindowDetails const&) override;
        virtual auto get_details() const -> WindowDetails const& override;

        virtual void attach(Keyboard*) override;
        virtual void detach(Keyboard*) override;

        virtual void attach(Mouse*) override;
        virtual void detach(Mouse*) override;

    protected:
        virtual void init() override;
        virtual void tick() override;
        virtual void dispose() override;

    private:
        GLFWwindow* m_WindowHandle{};
        GlfwContext m_GlfwContext{};

        GraphicsThread m_GraphicsThread{};
        RenderContext m_RenderContext;
        WindowDetails m_Details;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WINDOW_GLFW_ */


#include "RockHopper/UI/Window/GLFW.Context.hh"

#include "RockHopper/Logging/LoggerCore.enable.hh"
#include "RockHopper/Logging/Asserts.hh"

#include <GLFW/glfw3.h>

namespace RockHopper::UI::GLFW
{

    static void GLFW_ErrorCallback(int error, const char* description)
    {
        ROCKHOPPER_LOG_ERROR("GLFW Error {}: {}", error, description);
    }

    Window::Context::~Context()
    {
        try
        {
            auto task = m_Renderer.push_task([]()
            {
                ROCKHOPPER_LOG_INFO("GLFW terminating");
                glfwTerminate();
            });
            task.wait();
        }
        catch(std::exception const& e)
        {
            ROCKHOPPER_LOG_FATAL("Exception occurred on GLFW Context destruction.\n{}",e.what());
        }
    }

    Window::Context::Context()
    {
        auto task = m_Renderer.push_task([]()
        {
            ROCKHOPPER_LOG_INFO("GLFW initializing");
            glfwSetErrorCallback(GLFW_ErrorCallback);

            int const status = glfwInit();
            ROCKHOPPER_ASSERT_FATAL(status,"GLFW initialization failed");

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,s_VersionMajor);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,s_VersionMinor);
        });
    }

    void Window::Context::create_window(Window::Handle** handle) noexcept
    {
        try
        {
            auto future = m_Renderer.push_task([=]()
            {
                static constexpr int wid = 800;
                static constexpr int hei = 600;
                *handle = glfwCreateWindow(wid,hei,"RockHopper",nullptr,nullptr);
            });
            future.wait();
        }
        catch (std::exception const& e)
        {
            ROCKHOPPER_LOG_FATAL("GLFW encountered an error when creating a window.\n{}",e.what());
        }
    }

    void Window::Context::dispose_window(Window::Handle** handle) noexcept
    {
        try
        {
            auto future = m_Renderer.push_task([=]()
            {
                if ((bool)*handle) glfwDestroyWindow(static_cast<GLFWwindow*>(*handle));
            });
            future.wait();
        }
        catch (std::exception const& e)
        {
            ROCKHOPPER_LOG_FATAL("GLFW encountered an error when disposing a window.\n{}",e.what());
        }
    }

} // namespace RockHopper::UI::GLFW

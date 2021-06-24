
#include "GLFW_Context.hh"

#include "RockHopper/Debug.hh"
#include "RockHopper/Window/Window.hh"
#include "RockHopper/Input/Keyboard.hh"

#include <GLFW/glfw3.h>

/* ************************************************************************** */
// [Definition] RockHopper::GLFW_Context
/* ************************************************************************** */

namespace RockHopper
{

    void GLFW_Context::Register()
    {
        ROCKHOPPER_INTERNAL_ASSERT_FATAL((not m_Registered),"Trying to initialize GLFW more than once!");

        int status = glfwInit();
        ROCKHOPPER_INTERNAL_ASSERT_FATAL((status == GLFW_TRUE),"Failed to initialize GLFW!");
        ROCKHOPPER_INTERNAL_LOG_INFO("Initialized GLFW.");
        m_Registered = true;
    }

    void GLFW_Context::Deregister()
    {
        ROCKHOPPER_INTERNAL_ASSERT_FATAL(m_Registered,"Trying to terminate non-existent GLFW!");

        glfwTerminate();
        ROCKHOPPER_INTERNAL_LOG_INFO("Terminated GLFW.");
        m_Registered = false;
    }

} // namespace RockHopper

/* ************************************************************************** */
// [Definition] RockHopper::SetWindowGLFWCallbacks
/* ************************************************************************** */

namespace RockHopper
{

    template <>
    void SetWindowGLFWCallbacks<false>(GLFWwindow* handle)
    {
        glfwSetWindowPosCallback(handle,[](GLFWwindow* handle, int x, int y)
        {
        });
        glfwSetWindowSizeCallback(handle,[](GLFWwindow* handle, int width, int height)
        {
        });
        glfwSetWindowCloseCallback(handle,[](GLFWwindow* handle)
        {
        });
        glfwSetWindowRefreshCallback(handle,[](GLFWwindow* handle)
        {
        });
        glfwSetWindowFocusCallback(handle,[](GLFWwindow* handle, int focus)
        {
        });
        glfwSetWindowIconifyCallback(handle,[](GLFWwindow* handle, int iconified)
        {
        });
        glfwSetWindowMaximizeCallback(handle,[](GLFWwindow* handle, int maximized)
        {
        });
        glfwSetWindowContentScaleCallback(handle,[](GLFWwindow* handle, float scale_x, float scale_y)
        {
        });
        glfwSetFramebufferSizeCallback(handle,[](GLFWwindow* handle, int width, int height)
        {
        });
    }

    template <>
    void SetWindowGLFWCallbacks<true>(GLFWwindow* handle)
    {
        glfwSetWindowPosCallback(handle,[](GLFWwindow* handle, int x, int y)
        {
            WindowPositionEvent event;
            event.x = x;
            event.y = y;

            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            event.window = window;
            window->WindowEventHandler::dispatch_event(event);
        });
        glfwSetWindowSizeCallback(handle,[](GLFWwindow* handle, int width, int height)
        {
            WindowSizeEvent event;
            event.width = width;
            event.height = height;

            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            event.window = window;
            window->WindowEventHandler::dispatch_event(event);
        });
        glfwSetWindowCloseCallback(handle,[](GLFWwindow* handle)
        {
            WindowCloseEvent event;

            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            event.window = window;
            window->WindowEventHandler::dispatch_event(event);
        });
        glfwSetWindowRefreshCallback(handle,[](GLFWwindow* handle)
        {
            WindowRefreshEvent event;

            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            event.window = window;
            window->WindowEventHandler::dispatch_event(event);
        });
        glfwSetWindowFocusCallback(handle,[](GLFWwindow* handle, int focus)
        {
            WindowFocusEvent event;
            event.focus = focus;

            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            event.window = window;
            window->WindowEventHandler::dispatch_event(event);
        });
        glfwSetWindowIconifyCallback(handle,[](GLFWwindow* handle, int iconified)
        {
            WindowMinimizeEvent event;
            event.minimized = iconified;

            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            event.window = window;
            window->WindowEventHandler::dispatch_event(event);
        });
        glfwSetWindowMaximizeCallback(handle,[](GLFWwindow* handle, int maximized)
        {
            WindowMaximizedEvent event;
            event.maximized = maximized;

            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            event.window = window;
            window->WindowEventHandler::dispatch_event(event);
        });
        glfwSetWindowContentScaleCallback(handle,[](GLFWwindow* handle, float scale_x, float scale_y)
        {
            WindowScaleEvent event;
            event.scale_x = scale_x;
            event.scale_y = scale_y;

            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            event.window = window;
            window->WindowEventHandler::dispatch_event(event);
        });
        glfwSetFramebufferSizeCallback(handle,[](GLFWwindow* handle, int width, int height)
        {
            WindowBufferSizeEvent event;
            event.width = width;
            event.height = height;

            Window* window = (Window*)glfwGetWindowUserPointer(handle);
            event.window = window;
            window->WindowEventHandler::dispatch_event(event);
        });
    }

} // namespace RockHopper

/* ************************************************************************** */
// [Definition] RockHopper::SetKeyboardGLFWCallbacks
/* ************************************************************************** */

namespace RockHopper
{

    template <>
    void SetKeyboardGLFWCallbacks<false>(GLFWwindow* handle)
    {
        glfwSetKeyCallback(handle,[](GLFWwindow* handle, int keycode, int scancode, int action, int mods)
        {
        });
    }

    template <>
    void SetKeyboardGLFWCallbacks<true>(GLFWwindow* handle)
    {
        glfwSetKeyCallback(handle,[](GLFWwindow* handle, int keycode, int scancode, int action, int mods)
        {
            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressEvent event;
                    event.key = static_cast<KeyCode>(keycode);
                    event.mods = mods;
                    event.scancode = scancode;

                    Keyboard const* keyboard = ((Window*)glfwGetWindowUserPointer(handle))->keyboard();
                    event.keyboard = keyboard;
                    keyboard->dispatch_event(event);
                }
                break;
                case GLFW_RELEASE:
                {
                    KeyReleaseEvent event;
                    event.key = static_cast<KeyCode>(keycode);
                    event.mods = mods;
                    event.scancode = scancode;

                    Keyboard const* keyboard = ((Window*)glfwGetWindowUserPointer(handle))->keyboard();
                    event.keyboard = keyboard;
                    keyboard->dispatch_event(event);
                }
                break;
                case GLFW_REPEAT:
                {
                    KeyRepeatEvent event;
                    event.key = static_cast<KeyCode>(keycode);
                    event.mods = mods;
                    event.scancode = scancode;

                    Keyboard const* keyboard = ((Window*)glfwGetWindowUserPointer(handle))->keyboard();
                    event.keyboard = keyboard;
                    keyboard->dispatch_event(event);
                }
                break;
                default: ROCKHOPPER_INTERNAL_LOG_ERROR("Encountered an unknown GLFW key event!");
            }
        });
    }

} // namespace RockHopper

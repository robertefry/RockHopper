
#include "RockHopper/Window/Backend/GLFW_Context.hh"

#include "RockHopper/Debug.hh"
#include "RockHopper/Window/Window.hh"
#include "RockHopper/Input/Keyboard/Keyboard.hh"
#include "RockHopper/Input/Mouse/Mouse.hh"

#include <GLFW/glfw3.h>

/* ************************************************************************** */
// [Definition] RockHopper::GLFW_Context
/* ************************************************************************** */

namespace RockHopper
{

    static void GLFW_ErrorCallback(int error, const char* description)
    {
        ROCKHOPPER_INTERNAL_LOG_ERROR("GLFW Error {}: {}", error, description);
    }

    void GLFW_Context::Register()
    {
        if (m_Registered == 0)
        {
            // Setup GLFW Error Callback
            glfwSetErrorCallback(GLFW_ErrorCallback);

            // Initialize GLFW
            int status = glfwInit();
            ROCKHOPPER_INTERNAL_ASSERT_FATAL(status,"Failed to initialize GLFW!");
            ROCKHOPPER_INTERNAL_LOG_INFO("Initialized GLFW.");

            // Setup GLFW Window Hints
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,ROCKHOPPER_GLFW_VERSION_MAJOR);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,ROCKHOPPER_GLFW_VERSION_MINOR);
        }
        m_Registered += 1;
    }

    void GLFW_Context::Deregister()
    {
        if (m_Registered == 1)
        {
            glfwTerminate();
            ROCKHOPPER_INTERNAL_LOG_INFO("Terminated GLFW.");
        }
        m_Registered -= 1;
    }

} // namespace RockHopper

/* ************************************************************************** */
// [Definition] RockHopper::SetWindowGLFWCallbacks
/* ************************************************************************** */

namespace RockHopper
{

    template <>
    void GLFW_Context::SetWindowGLFWCallbacks<false>(GLFWwindow* handle)
    {
        glfwSetWindowRefreshCallback(handle,[](GLFWwindow* handle)
        {
        });
        glfwSetWindowCloseCallback(handle,[](GLFWwindow* handle)
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
        glfwSetWindowPosCallback(handle,[](GLFWwindow* handle, int x, int y)
        {
        });
        glfwSetWindowSizeCallback(handle,[](GLFWwindow* handle, int width, int height)
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
    void GLFW_Context::SetWindowGLFWCallbacks<true>(GLFWwindow* handle)
    {
        glfwSetWindowRefreshCallback(handle,[](GLFWwindow* handle)
        {
            WindowRefreshEvent event;

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
        glfwSetWindowPosCallback(handle,[](GLFWwindow* handle, int x, int y)
        {
            WindowMoveEvent event;
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
    void GLFW_Context::SetKeyboardGLFWCallbacks<false>(GLFWwindow* handle)
    {
        glfwSetKeyCallback(handle,[](GLFWwindow* handle, int keycode, int scancode, int action, int mods)
        {
        });
    }

    template <>
    void GLFW_Context::SetKeyboardGLFWCallbacks<true>(GLFWwindow* handle)
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

                    Keyboard* keyboard = ((Window*)glfwGetWindowUserPointer(handle))->keyboard();
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

                    Keyboard* keyboard = ((Window*)glfwGetWindowUserPointer(handle))->keyboard();
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

                    Keyboard* keyboard = ((Window*)glfwGetWindowUserPointer(handle))->keyboard();
                    event.keyboard = keyboard;
                    keyboard->dispatch_event(event);
                }
                break;
                default: ROCKHOPPER_INTERNAL_LOG_ERROR("Encountered an unknown GLFW key event!");
            }
        });
    }

} // namespace RockHopper

/* ************************************************************************** */
// [Definition] RockHopper::SetKeyboardGLFWCallbacks
/* ************************************************************************** */

namespace RockHopper
{

    template <>
    void GLFW_Context::SetMouseGLFWCallbacks<false>(GLFWwindow* handle)
    {
        glfwSetCursorPosCallback(handle,[](GLFWwindow* handle, double x, double y)
        {
        });
        glfwSetCursorEnterCallback(handle,[](GLFWwindow* handle, int entered)
        {
        });
        glfwSetMouseButtonCallback(handle,[](GLFWwindow* handle, int button, int action, int mods)
        {
        });
        glfwSetScrollCallback(handle,[](GLFWwindow* handle, double offset_x, double offset_y)
        {
        });
    }

    template <>
    void GLFW_Context::SetMouseGLFWCallbacks<true>(GLFWwindow* handle)
    {
        glfwSetCursorPosCallback(handle,[](GLFWwindow* handle, double x, double y)
        {
            Mouse* mouse = ((Window*)glfwGetWindowUserPointer(handle))->mouse();

            if (mouse->key(MouseCode::BUTTON_LEFT).down())
            {
                MouseDragEvent event;
                event.x = x;
                event.y = y;

                event.mouse = mouse;
                mouse->dispatch_event(event);
            }
            else
            {
                MouseMoveEvent event;
                event.x = x;
                event.y = y;

                event.mouse = mouse;
                mouse->dispatch_event(event);
            }
        });
        glfwSetCursorEnterCallback(handle,[](GLFWwindow* handle, int entered)
        {
            switch (entered)
            {
                case GLFW_TRUE:
                {
                    MouseEnterEvent event;

                    Mouse* mouse = ((Window*)glfwGetWindowUserPointer(handle))->mouse();
                    event.mouse = mouse;
                    mouse->dispatch_event(event);
                }
                break;
                case GLFW_FALSE:
                {
                    MouseExitEvent event;

                    Mouse* mouse = ((Window*)glfwGetWindowUserPointer(handle))->mouse();
                    event.mouse = mouse;
                    mouse->dispatch_event(event);
                }
                break;
                default: ROCKHOPPER_INTERNAL_LOG_ERROR("Encountered an known GLFW mouse event!");
            }
        });
        glfwSetMouseButtonCallback(handle,[](GLFWwindow* handle, int button, int action, int mods)
        {
            switch (action)
            {
                case GLFW_PRESS:
                {
                    MousePressEvent event;
                    event.button = static_cast<MouseCode>(button);
                    event.mods = mods;

                    Mouse* mouse = ((Window*)glfwGetWindowUserPointer(handle))->mouse();
                    event.mouse = mouse;
                    mouse->dispatch_event(event);
                }
                break;
                case GLFW_RELEASE:
                {
                    MouseReleaseEvent event;
                    event.button = static_cast<MouseCode>(button);
                    event.mods = mods;

                    Mouse* mouse = ((Window*)glfwGetWindowUserPointer(handle))->mouse();
                    event.mouse = mouse;
                    mouse->dispatch_event(event);
                }
                break;
                case GLFW_REPEAT:
                {
                    MouseRepeatEvent event;
                    event.button = static_cast<MouseCode>(button);
                    event.mods = mods;

                    Mouse* mouse = ((Window*)glfwGetWindowUserPointer(handle))->mouse();
                    event.mouse = mouse;
                    mouse->dispatch_event(event);
                }
                break;
                default: ROCKHOPPER_INTERNAL_LOG_ERROR("Encountered an known GLFW mouse event!");
            }
        });
        glfwSetScrollCallback(handle,[](GLFWwindow* handle, double offset_x, double offset_y)
        {
            MouseScrollEvent event;
            event.offset_x = offset_x;
            event.offset_y = offset_y;

            Mouse* mouse = ((Window*)glfwGetWindowUserPointer(handle))->mouse();
            event.mouse = mouse;
            mouse->dispatch_event(event);
        });
    }

} // namespace RockHopper


#include "RockHopper/Window/Platform/GlfwContext.hh"

#include "RockHopper/Window/Window.hh"
#include "RockHopper/Input/Keyboard/Keyboard.hh"
#include "RockHopper/Input/Mouse/Mouse.hh"

#include <GLFW/glfw3.h>

#include <unordered_map>
#include <mutex>

/* ************************************************************************** */
// [Local Class] RockHopper::GlfwContextData
/* ************************************************************************** */

namespace RockHopper
{

    class GlfwContextData
    {
    public:

        template <typename T_Device>
        static void SetUserPointer(GLFWwindow* handle, T_Device* device);

        template <typename T_Device>
        static auto GetUserPointer(GLFWwindow* handle) -> T_Device*;

    private:
        struct Devices
        {
            Window* window;
            Keyboard* keyboard;
            Mouse* mouse;
        };
        static inline std::unordered_map<GLFWwindow*,Devices> s_DeviceMap{};
        static inline std::mutex s_Mutex{};
    };

    template <typename T_Device>
    void GlfwContextData::SetUserPointer(GLFWwindow* handle, T_Device* device)
    {
        std::unique_lock lock {s_Mutex};

        if constexpr (std::is_base_of<Window,T_Device>::value)
        {
            s_DeviceMap[handle].window = device; return;
        }
        if constexpr (std::is_base_of<Keyboard,T_Device>::value)
        {
            s_DeviceMap[handle].keyboard = device; return;
        }
        if constexpr (std::is_base_of<Mouse,T_Device>::value)
        {
            s_DeviceMap[handle].mouse = device; return;
        }

        // execution should never get here
        ROCKHOPPER_INTERNAL_LOG_FATAL("No matching user pointer for type");
    }

    template <typename T_Device>
    auto GlfwContextData::GetUserPointer(GLFWwindow* handle) -> T_Device*
    {
        std::unique_lock lock {s_Mutex};

        if constexpr (std::is_base_of<Window,T_Device>::value)
        {
            return s_DeviceMap.at(handle).window;
        }
        if constexpr (std::is_base_of<Keyboard,T_Device>::value)
        {
            return s_DeviceMap.at(handle).keyboard;
        }
        if constexpr (std::is_base_of<Mouse,T_Device>::value)
        {
            return s_DeviceMap.at(handle).mouse;
        }

        // execution should never get here
        ROCKHOPPER_INTERNAL_LOG_FATAL("No matching user pointer for type");
        return nullptr;
    }

} // namespace RockHopper

/* ************************************************************************** */
// [Implementation] RockHopper::GlfwContext
/* ************************************************************************** */

namespace RockHopper
{

    static void GLFW_ErrorCallback(int error, const char* description)
    {
        ROCKHOPPER_INTERNAL_LOG_ERROR("GLFW Error {}: {}", error, description);
    }

    GlfwContext::~GlfwContext()
    {
        if (s_NumInstances == 1)
        {
            m_RenderThread.wait_task([]()
            {
                ROCKHOPPER_INTERNAL_LOG_INFO("Terminating GLFW.");
                glfwTerminate();
            });
        }
        s_NumInstances -=1;
    }

    GlfwContext::GlfwContext()
    {
        if (s_NumInstances == 0)
        {
            m_RenderThread.wait_task([]()
            {
                ROCKHOPPER_INTERNAL_LOG_INFO("Initializing GLFW.");
                glfwSetErrorCallback(GLFW_ErrorCallback);

                int status = glfwInit();
                ROCKHOPPER_INTERNAL_ASSERT_FATAL(status,"Failed to initialize GLFW!");

                // Setup GLFW Window Hints
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,ROCKHOPPER_GLFW_VERSION_MAJOR);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,ROCKHOPPER_GLFW_VERSION_MINOR);
            });
        }
        s_NumInstances += 1;
    }

    GlfwContext::GlfwContext(GlfwContext const&)
    {
        s_NumInstances += 1;
    }

    GlfwContext& GlfwContext::operator=(GlfwContext const&)
    {
        s_NumInstances += 1;
        return *this;
    }

    template <>
    std::future<void> GlfwContext::set_callbacks<Window,false>(GLFWwindow* handle, Window* window)
    {
        return m_RenderThread.push_task([handle,window]()
        {
            GlfwContextData::SetUserPointer<Window>(handle,window);

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
        });
    }

    template <>
    std::future<void> GlfwContext::set_callbacks<Window,true>(GLFWwindow* handle, Window* window)
    {
        return m_RenderThread.push_task([handle,window]()
        {
            GlfwContextData::SetUserPointer<Window>(handle,window);

            glfwSetWindowRefreshCallback(handle,[](GLFWwindow* handle)
            {
                WindowRefreshEvent event;

                Window* window = GlfwContextData::GetUserPointer<Window>(handle);
                event.window = window;
                window->dispatch_event(event);
            });
            glfwSetWindowCloseCallback(handle,[](GLFWwindow* handle)
            {
                WindowCloseEvent event;

                Window* window = GlfwContextData::GetUserPointer<Window>(handle);
                event.window = window;
                window->dispatch_event(event);
            });
            glfwSetWindowFocusCallback(handle,[](GLFWwindow* handle, int focus)
            {
                WindowFocusEvent event;
                event.focus = focus;

                Window* window = GlfwContextData::GetUserPointer<Window>(handle);
                event.window = window;
                window->dispatch_event(event);
            });
            glfwSetWindowIconifyCallback(handle,[](GLFWwindow* handle, int iconified)
            {
                WindowMinimizeEvent event;
                event.minimized = iconified;

                Window* window = GlfwContextData::GetUserPointer<Window>(handle);
                event.window = window;
                window->dispatch_event(event);
            });
            glfwSetWindowMaximizeCallback(handle,[](GLFWwindow* handle, int maximized)
            {
                WindowMaximizedEvent event;
                event.maximized = maximized;

                Window* window = GlfwContextData::GetUserPointer<Window>(handle);
                event.window = window;
                window->dispatch_event(event);
            });
            glfwSetWindowPosCallback(handle,[](GLFWwindow* handle, int x, int y)
            {
                WindowMoveEvent event;
                event.x = x;
                event.y = y;

                Window* window = GlfwContextData::GetUserPointer<Window>(handle);
                event.window = window;
                window->dispatch_event(event);
            });
            glfwSetWindowSizeCallback(handle,[](GLFWwindow* handle, int width, int height)
            {
                WindowSizeEvent event;
                event.width = width;
                event.height = height;

                Window* window = GlfwContextData::GetUserPointer<Window>(handle);
                event.window = window;
                window->dispatch_event(event);
            });
            glfwSetWindowContentScaleCallback(handle,[](GLFWwindow* handle, float scale_x, float scale_y)
            {
                WindowScaleEvent event;
                event.scale_x = scale_x;
                event.scale_y = scale_y;

                Window* window = GlfwContextData::GetUserPointer<Window>(handle);
                event.window = window;
                window->dispatch_event(event);
            });
            glfwSetFramebufferSizeCallback(handle,[](GLFWwindow* handle, int width, int height)
            {
                WindowBufferSizeEvent event;
                event.width = width;
                event.height = height;

                Window* window = GlfwContextData::GetUserPointer<Window>(handle);
                event.window = window;
                window->dispatch_event(event);
            });
        });
    }

    template <>
    std::future<void> GlfwContext::set_callbacks<Keyboard,false>(GLFWwindow* handle, Keyboard* keyboard)
    {
        return m_RenderThread.push_task([handle,keyboard]()
        {
            GlfwContextData::SetUserPointer<Keyboard>(handle,keyboard);

            glfwSetKeyCallback(handle,[](GLFWwindow* handle, int keycode, int scancode, int action, int mods)
            {
            });
        });
    }

    template <>
    std::future<void> GlfwContext::set_callbacks<Keyboard,true>(GLFWwindow* handle, Keyboard* keyboard)
    {
        return m_RenderThread.push_task([handle,keyboard]()
        {
            GlfwContextData::SetUserPointer<Keyboard>(handle,keyboard);

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

                        Keyboard* keyboard = GlfwContextData::GetUserPointer<Keyboard>(handle);
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

                        Keyboard* keyboard = GlfwContextData::GetUserPointer<Keyboard>(handle);
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

                        Keyboard* keyboard = GlfwContextData::GetUserPointer<Keyboard>(handle);
                        event.keyboard = keyboard;
                        keyboard->dispatch_event(event);
                    }
                    break;
                    default: ROCKHOPPER_INTERNAL_LOG_ERROR("Encountered an unknown GLFW key event!");
                }
            });
        });
    }

    template <>
    std::future<void> GlfwContext::set_callbacks<Mouse,false>(GLFWwindow* handle, Mouse* mouse)
    {
        return m_RenderThread.push_task([handle,mouse]()
        {
            GlfwContextData::SetUserPointer<Mouse>(handle,mouse);

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
        });
    }

    template <>
    std::future<void> GlfwContext::set_callbacks<Mouse,true>(GLFWwindow* handle, Mouse* mouse)
    {
        return m_RenderThread.push_task([handle,mouse]()
        {
            GlfwContextData::SetUserPointer<Mouse>(handle,mouse);

            glfwSetCursorPosCallback(handle,[](GLFWwindow* handle, double x, double y)
            {
                Mouse* mouse = GlfwContextData::GetUserPointer<Mouse>(handle);

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

                        Mouse* mouse = GlfwContextData::GetUserPointer<Mouse>(handle);
                        event.mouse = mouse;
                        mouse->dispatch_event(event);
                    }
                    break;
                    case GLFW_FALSE:
                    {
                        MouseExitEvent event;

                        Mouse* mouse = GlfwContextData::GetUserPointer<Mouse>(handle);
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

                        Mouse* mouse = GlfwContextData::GetUserPointer<Mouse>(handle);
                        event.mouse = mouse;
                        mouse->dispatch_event(event);
                    }
                    break;
                    case GLFW_RELEASE:
                    {
                        MouseReleaseEvent event;
                        event.button = static_cast<MouseCode>(button);
                        event.mods = mods;

                        Mouse* mouse = GlfwContextData::GetUserPointer<Mouse>(handle);
                        event.mouse = mouse;
                        mouse->dispatch_event(event);
                    }
                    break;
                    case GLFW_REPEAT:
                    {
                        MouseRepeatEvent event;
                        event.button = static_cast<MouseCode>(button);
                        event.mods = mods;

                        Mouse* mouse = GlfwContextData::GetUserPointer<Mouse>(handle);
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

                Mouse* mouse = GlfwContextData::GetUserPointer<Mouse>(handle);
                event.mouse = mouse;
                mouse->dispatch_event(event);
            });
        });
    }

} // namespace RockHopper

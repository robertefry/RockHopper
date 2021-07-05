
#include "RockHopper/Window/Backend/WindowContext.hh"

#include "RockHopper/Debug.hh"
#include "RockHopper/Window/Window.hh"
#include "RockHopper/Input/Keyboard/Keyboard.hh"
#include "RockHopper/Input/Mouse/Mouse.hh"

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <mutex>

/* ************************************************************************** */
// [Local Class] RockHopper::WindowContextData
/* ************************************************************************** */

namespace RockHopper
{

    class WindowContextData
    {
    public:
        static void ClearUserPointers(void*);

        template <typename T_Type>
        static void SetUserPointer(void*, T_Type*);

        template <typename T_Type>
        static T_Type* GetUserPointer(void*);

    private:
        struct MapContent
        {
            Window* window;
            Keyboard* keyboard;
            Mouse* mouse;
        };
        static inline std::unordered_map<void*,MapContent> s_ContentMap{};
        static inline std::mutex s_ContentMapMutex{};
    };

    void WindowContextData::ClearUserPointers(void* handle)
    {
        s_ContentMap.erase(handle);
    }

    template <typename T_Type>
    void WindowContextData::SetUserPointer(void* handle, T_Type* data)
    {
        std::unique_lock<std::mutex> lock {s_ContentMapMutex};

        if constexpr (std::is_same<T_Type,Window>::value)
        {
            s_ContentMap[handle].window = data;
        }
        if constexpr (std::is_same<T_Type,Keyboard>::value)
        {
            s_ContentMap[handle].keyboard = data;
        }
        if constexpr (std::is_same<T_Type,Mouse>::value)
        {
            s_ContentMap[handle].mouse = data;
        }
    }

    template <typename T_Type>
    T_Type* WindowContextData::GetUserPointer(void* handle)
    {
        std::unique_lock<std::mutex> lock {s_ContentMapMutex};

        if constexpr (std::is_same<T_Type,Window>::value)
        {
            return s_ContentMap.at(handle).window;
        }
        if constexpr (std::is_same<T_Type,Keyboard>::value)
        {
            return s_ContentMap.at(handle).keyboard;
        }
        if constexpr (std::is_same<T_Type,Mouse>::value)
        {
            return s_ContentMap.at(handle).mouse;
        }

        // execution should never get here
        ROCKHOPPER_INTERNAL_LOG_FATAL("No matching user pointer data found for type {}.", typeid(T_Type).name());
        return nullptr;
    }

} // namespace RockHopper

/* ************************************************************************** */
// [Definition] RockHopper::WindowContext
/* ************************************************************************** */

namespace RockHopper
{

    static void GLFW_ErrorCallback(int error, const char* description)
    {
        ROCKHOPPER_INTERNAL_LOG_ERROR("GLFW Error {}: {}", error, description);
    }

    WindowContext::~WindowContext()
    {
        if (s_NumInstances == 1)
        {
            m_GraphicsThread.wait_task([]()
            {
                glfwTerminate();
                ROCKHOPPER_INTERNAL_LOG_INFO("Terminated GLFW.");
            });
        }
        s_NumInstances -= 1;
    }

    WindowContext::WindowContext(GraphicsThread const& thread, void** handle)
        : m_GraphicsThread{thread}, m_Handle{handle}
    {
        if (s_NumInstances == 0)
        {
            m_GraphicsThread.wait_task([]()
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
            });
        }
        s_NumInstances += 1;
    }

    WindowContext::WindowContext(WindowContext const& other)
        : m_GraphicsThread{other.m_GraphicsThread}
        , m_Handle{other.m_Handle}
    {
        s_NumInstances += 1;
    }

    WindowContext& WindowContext::operator=(WindowContext const& other)
    {
        m_GraphicsThread = other.m_GraphicsThread;
        m_Handle = other.m_Handle;
        return *this;
    }

    template <>
    void WindowContext::set_callbacks<Window,false>(Window* window)
    {
        m_GraphicsThread.push_task([this,window]()
        {
            WindowContextData::SetUserPointer<Window>(*m_Handle,window);

            glfwSetWindowRefreshCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle)
            {
            });
            glfwSetWindowCloseCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle)
            {
            });
            glfwSetWindowFocusCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, int focus)
            {
            });
            glfwSetWindowIconifyCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, int iconified)
            {
            });
            glfwSetWindowMaximizeCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, int maximized)
            {
            });
            glfwSetWindowPosCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, int x, int y)
            {
            });
            glfwSetWindowSizeCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, int width, int height)
            {
            });
            glfwSetWindowContentScaleCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, float scale_x, float scale_y)
            {
            });
            glfwSetFramebufferSizeCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, int width, int height)
            {
            });
        });
    }

    template <>
    void WindowContext::set_callbacks<Window,true>(Window* window)
    {
        m_GraphicsThread.push_task([this,window]()
        {
            WindowContextData::SetUserPointer<Window>(*m_Handle,window);

            glfwSetWindowRefreshCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle)
            {
                WindowRefreshEvent event;

                Window* window = WindowContextData::GetUserPointer<Window>(handle);
                event.window = window;
                window->dispatch_event(event);
            });
            glfwSetWindowCloseCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle)
            {
                WindowCloseEvent event;

                Window* window = WindowContextData::GetUserPointer<Window>(handle);
                event.window = window;
                window->dispatch_event(event);
            });
            glfwSetWindowFocusCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, int focus)
            {
                WindowFocusEvent event;
                event.focus = focus;

                Window* window = WindowContextData::GetUserPointer<Window>(handle);
                event.window = window;
                window->dispatch_event(event);
            });
            glfwSetWindowIconifyCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, int iconified)
            {
                WindowMinimizeEvent event;
                event.minimized = iconified;

                Window* window = WindowContextData::GetUserPointer<Window>(handle);
                event.window = window;
                window->dispatch_event(event);
            });
            glfwSetWindowMaximizeCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, int maximized)
            {
                WindowMaximizedEvent event;
                event.maximized = maximized;

                Window* window = WindowContextData::GetUserPointer<Window>(handle);
                event.window = window;
                window->dispatch_event(event);
            });
            glfwSetWindowPosCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, int x, int y)
            {
                WindowMoveEvent event;
                event.x = x;
                event.y = y;

                Window* window = WindowContextData::GetUserPointer<Window>(handle);
                event.window = window;
                window->dispatch_event(event);
            });
            glfwSetWindowSizeCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, int width, int height)
            {
                WindowSizeEvent event;
                event.width = width;
                event.height = height;

                Window* window = WindowContextData::GetUserPointer<Window>(handle);
                event.window = window;
                window->dispatch_event(event);
            });
            glfwSetWindowContentScaleCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, float scale_x, float scale_y)
            {
                WindowScaleEvent event;
                event.scale_x = scale_x;
                event.scale_y = scale_y;

                Window* window = WindowContextData::GetUserPointer<Window>(handle);
                event.window = window;
                window->dispatch_event(event);
            });
            glfwSetFramebufferSizeCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, int width, int height)
            {
                WindowBufferSizeEvent event;
                event.width = width;
                event.height = height;

                Window* window = WindowContextData::GetUserPointer<Window>(handle);
                event.window = window;
                window->dispatch_event(event);
            });
        });
    }

    template <>
    void WindowContext::set_callbacks<Keyboard,false>(Keyboard* keyboard)
    {
        m_GraphicsThread.push_task([this,keyboard]()
        {
            WindowContextData::SetUserPointer<Keyboard>(*m_Handle,keyboard);

            glfwSetKeyCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, int keycode, int scancode, int action, int mods)
            {
            });
        });
    }

    template <>
    void WindowContext::set_callbacks<Keyboard,true>(Keyboard* keyboard)
    {
        m_GraphicsThread.push_task([this,keyboard]()
        {
            WindowContextData::SetUserPointer<Keyboard>(*m_Handle,keyboard);

            glfwSetKeyCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, int keycode, int scancode, int action, int mods)
            {
                switch (action)
                {
                    case GLFW_PRESS:
                    {
                        KeyPressEvent event;
                        event.key = static_cast<KeyCode>(keycode);
                        event.mods = mods;
                        event.scancode = scancode;

                        Keyboard* keyboard = WindowContextData::GetUserPointer<Keyboard>(handle);
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

                        Keyboard* keyboard = WindowContextData::GetUserPointer<Keyboard>(handle);
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

                        Keyboard* keyboard = WindowContextData::GetUserPointer<Keyboard>(handle);
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
    void WindowContext::set_callbacks<Mouse,false>(Mouse* mouse)
    {
        m_GraphicsThread.push_task([this,mouse]()
        {
            WindowContextData::SetUserPointer<Mouse>(*m_Handle,mouse);

            glfwSetCursorPosCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, double x, double y)
            {
            });
            glfwSetCursorEnterCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, int entered)
            {
            });
            glfwSetMouseButtonCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, int button, int action, int mods)
            {
            });
            glfwSetScrollCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, double offset_x, double offset_y)
            {
            });
        });
    }

    template <>
    void WindowContext::set_callbacks<Mouse,true>(Mouse* mouse)
    {
        m_GraphicsThread.push_task([this,mouse]()
        {
            WindowContextData::SetUserPointer<Mouse>(*m_Handle,mouse);

            glfwSetCursorPosCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, double x, double y)
            {
                Mouse* mouse = WindowContextData::GetUserPointer<Mouse>(handle);

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
            glfwSetCursorEnterCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, int entered)
            {
                switch (entered)
                {
                    case GLFW_TRUE:
                    {
                        MouseEnterEvent event;

                        Mouse* mouse = WindowContextData::GetUserPointer<Mouse>(handle);
                        event.mouse = mouse;
                        mouse->dispatch_event(event);
                    }
                    break;
                    case GLFW_FALSE:
                    {
                        MouseExitEvent event;

                        Mouse* mouse = WindowContextData::GetUserPointer<Mouse>(handle);
                        event.mouse = mouse;
                        mouse->dispatch_event(event);
                    }
                    break;
                    default: ROCKHOPPER_INTERNAL_LOG_ERROR("Encountered an known GLFW mouse event!");
                }
            });
            glfwSetMouseButtonCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, int button, int action, int mods)
            {
                switch (action)
                {
                    case GLFW_PRESS:
                    {
                        MousePressEvent event;
                        event.button = static_cast<MouseCode>(button);
                        event.mods = mods;

                        Mouse* mouse = WindowContextData::GetUserPointer<Mouse>(handle);
                        event.mouse = mouse;
                        mouse->dispatch_event(event);
                    }
                    break;
                    case GLFW_RELEASE:
                    {
                        MouseReleaseEvent event;
                        event.button = static_cast<MouseCode>(button);
                        event.mods = mods;

                        Mouse* mouse = WindowContextData::GetUserPointer<Mouse>(handle);
                        event.mouse = mouse;
                        mouse->dispatch_event(event);
                    }
                    break;
                    case GLFW_REPEAT:
                    {
                        MouseRepeatEvent event;
                        event.button = static_cast<MouseCode>(button);
                        event.mods = mods;

                        Mouse* mouse = WindowContextData::GetUserPointer<Mouse>(handle);
                        event.mouse = mouse;
                        mouse->dispatch_event(event);
                    }
                    break;
                    default: ROCKHOPPER_INTERNAL_LOG_ERROR("Encountered an known GLFW mouse event!");
                }
            });
            glfwSetScrollCallback((GLFWwindow*)*m_Handle,[](GLFWwindow* handle, double offset_x, double offset_y)
            {
                MouseScrollEvent event;
                event.offset_x = offset_x;
                event.offset_y = offset_y;

                Mouse* mouse = WindowContextData::GetUserPointer<Mouse>(handle);
                event.mouse = mouse;
                mouse->dispatch_event(event);
            });
        });
    }

} // namespace RockHopper

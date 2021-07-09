
#include "RockHopper/Window/Window.hh"

#include "RockHopper/Debug.hh"
#include "RockHopper/Input/Keyboard/Keyboard.hh"
#include "RockHopper/Input/Mouse/Mouse.hh"

#include <GLFW/glfw3.h>

#include <utility>

namespace RockHopper
{

    Window::~Window()
    {
    }

    Window::Window(WindowDetails const& details)
        : Engine{details.title}
        , m_GraphicsThread{}
        , m_WindowContext{m_GraphicsThread}
        , m_RenderContext{m_GraphicsThread}
        , m_Details{details}
    {
        m_DebugName.set_type("Window");
        set_details(details);
    }

    void Window::set_details(WindowDetails const& details)
    {
        insert_task([this,details]() { m_GraphicsThread.push_task([this,details]()
        {
            m_Timing.set_omega(details.frametime);

            ROCKHOPPER_INTERNAL_LOG_INFO("Setting details for {}.", m_DebugName);
            GLFWwindow* handle = m_WindowContext.get_window();
            if (handle)
            {
                glfwSetWindowSize(handle,details.width,details.height);
                glfwSetWindowTitle(handle,details.title.c_str());
            }
            m_Details = details;
        }); });
    }

    auto Window::get_details() const -> WindowDetails const&
    {
        return m_Details;
    }

    void Window::attach(Keyboard* keyboard)
    {
        insert_task([this,keyboard]() { m_GraphicsThread.wait_task([this,keyboard]()
        {
            ROCKHOPPER_INTERNAL_LOG_INFO("Attaching the Keyboard {} to the Window {}.", keyboard->m_DebugName, m_DebugName);
            if (m_KeyboardHandle != nullptr)
            {
                ROCKHOPPER_INTERNAL_LOG_ERROR("The Keyboard {} is already attached to the Window {}!", keyboard->m_DebugName, m_DebugName);
                return;
            }
            if (keyboard->m_WindowHandle != nullptr)
            {
                ROCKHOPPER_INTERNAL_LOG_ERROR("The Window {} is already attached to the Keyboard {}!", m_DebugName, keyboard->m_DebugName);
                return;
            }
            m_KeyboardHandle = keyboard;
            keyboard->m_WindowHandle = this;
            m_WindowContext.set_callbacks<Keyboard,true>(keyboard);
        }); });
    }

    void Window::detach(Keyboard* keyboard)
    {
        insert_task([this,keyboard]() { m_GraphicsThread.wait_task([this,keyboard]()
        {
            ROCKHOPPER_INTERNAL_LOG_INFO("Detaching the Keyboard {} from the Window {}.", keyboard->m_DebugName, m_DebugName);
            m_WindowContext.set_callbacks<Keyboard,false>(keyboard);
            keyboard->m_WindowHandle = nullptr;
            m_KeyboardHandle = nullptr;
        }); });
    }

    void Window::attach(Mouse* mouse)
    {
        insert_task([this,mouse]() { m_GraphicsThread.wait_task([this,mouse]()
        {
            ROCKHOPPER_INTERNAL_LOG_INFO("Attaching the Mouse {} to the Window {}.", mouse->m_DebugName, m_DebugName);
            if (m_MouseHandle != nullptr)
            {
                ROCKHOPPER_INTERNAL_LOG_ERROR("The Mouse {} is already attached to the Window {}!", mouse->m_DebugName, m_DebugName);
                return;
            }
            if (mouse->m_WindowHandle != nullptr)
            {
                ROCKHOPPER_INTERNAL_LOG_ERROR("The Window {} is already attached to the Mouse {}!", m_DebugName, mouse->m_DebugName);
                return;
            }
            m_MouseHandle = mouse;
            mouse->m_WindowHandle = this;
            m_WindowContext.set_callbacks<Mouse,true>(mouse);
        }); });
    }

    void Window::detach(Mouse* mouse)
    {
        insert_task([this,mouse]() { m_GraphicsThread.wait_task([this,mouse]()
        {
            ROCKHOPPER_INTERNAL_LOG_INFO("Detached the Mouse {} from the Window {}.", mouse->m_DebugName, m_DebugName);
            m_WindowContext.set_callbacks<Mouse,false>(mouse);
            mouse->m_WindowHandle = nullptr;
            m_MouseHandle = nullptr;
        }); });
    }

    void Window::init()
    {
        m_GraphicsThread.wait_task([this]()
        {
            // Create a GLFW windowed-mode window handle and it's OpenGL context
            GLFWwindow* handle = glfwCreateWindow(m_Details.width,m_Details.height,m_Details.title.c_str(),NULL,NULL);
            ROCKHOPPER_INTERNAL_ASSERT_FATAL(handle,"Failed to create a GLFW window handle! {}", m_DebugName);
            ROCKHOPPER_INTERNAL_LOG_DEBUG("Created a GLFW window. {}", m_DebugName);
            m_WindowContext.set_window(handle);

            // Set the GLFW user pointer to this window
            glfwSetWindowUserPointer(m_WindowContext.get_window(),this);
        });

        // Set GLFW callbacks
        m_WindowContext.set_callbacks<Window,true>(this);

        // Initialize the render context
        m_RenderContext.initialize(m_WindowContext.get_window());

        // Dispatch a `WindowInitEvent` event
        m_GraphicsThread.wait_task([this]()
        {
            WindowInitEvent event;
            event.window = this;
            dispatch_event(event);
        });
    }

    void Window::tick()
    {
        if (m_WindowContext.is_closed_requested())
        {
            stop();
        }

        // Refresh the window context
        if (m_KeyboardHandle) m_KeyboardHandle->tick();
        if (m_MouseHandle) m_MouseHandle->tick();
        m_WindowContext.refresh();

        // Refresh the render context
        m_RenderContext.refresh(m_WindowContext.get_window());

        // Dispatch a `WindowRefreshEvent` event
        m_GraphicsThread.wait_task([this]()
        {
            WindowRefreshEvent event;
            event.window = this;
            dispatch_event(event);
        });

        // Wait on a blank graphics task to synchronize the graphics thread
        // with the current window thread.
        m_GraphicsThread.wait_task([](){});
    }

    void Window::dispose()
    {
        // Dispatch a `WindowDisposeEvent` event
        m_GraphicsThread.wait_task([this]()
        {
            WindowDisposeEvent event;
            event.window = this;
            dispatch_event(event);
        });

        // Dispose the render context
        m_RenderContext.dispose(m_WindowContext.get_window());

        // Unset GLFW callbacks
        m_WindowContext.set_callbacks<Window,false>(this);

        m_GraphicsThread.wait_task([this]()
        {
            // Remove the GLFW user pointer
            glfwSetWindowUserPointer(m_WindowContext.get_window(),nullptr);

            // Destroy the GLFW window
            glfwDestroyWindow(m_WindowContext.get_window());
            m_WindowContext.set_window(nullptr);
        });
    }

} // namespace RockHopper

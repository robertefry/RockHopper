
#include "RockHopper/Window/Window.hh"
#include "RockHopper/Window/Backend/GLFW_Context.hh"
#include "RockHopper/Window/Backend/OpenGL_Context.hh"

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
        : m_WindowHandle{}
        , m_Details{details}
    {
    }

    void Window::set_details(WindowDetails const& details)
    {
        insert_task([this,details]() { m_GraphicsThread.push_task([this,details]()
        {
            std::lock_guard<std::mutex> lock {m_WindowMutex};
            m_Timing.set_omega(details.frametime);

            ROCKHOPPER_INTERNAL_LOG_INFO("Setting details for a window.");
            if (m_WindowHandle)
            {
                glfwSetWindowSize(m_WindowHandle,details.width,details.height);
                glfwSetWindowTitle(m_WindowHandle,details.title.c_str());
            }
            m_Details = details;
        }); });
    }

    auto Window::get_details() const -> WindowDetails const&
    {
        std::lock_guard<std::mutex> lock {m_WindowMutex};
        return m_Details;
    }

    void Window::attach(Keyboard* keyboard)
    {
        insert_task([this,keyboard]() { m_GraphicsThread.wait_task([this,keyboard]()
        {
            if (m_KeyboardHandle != nullptr)
            {
                ROCKHOPPER_INTERNAL_LOG_ERROR("A `Keyboard` is already attached to this `Window`!");
                return;
            }
            if (keyboard->m_WindowHandle != nullptr)
            {
                ROCKHOPPER_INTERNAL_LOG_ERROR("A `Window` is already attached to this `Keyboard`!");
                return;
            }
            m_KeyboardHandle = keyboard;
            keyboard->m_WindowHandle = this;
            GLFW_Context::SetKeyboardGLFWCallbacks<true>(m_WindowHandle);
            ROCKHOPPER_INTERNAL_LOG_INFO("Attached a `Keyboard` to a `Window`.");
        }); });
    }

    void Window::detach(Keyboard* keyboard)
    {
        insert_task([this,keyboard]() { m_GraphicsThread.wait_task([this,keyboard]()
        {
            GLFW_Context::SetKeyboardGLFWCallbacks<false>(m_WindowHandle);
            keyboard->m_WindowHandle = nullptr;
            m_KeyboardHandle = nullptr;
            ROCKHOPPER_INTERNAL_LOG_INFO("Detached a `Keyboard` to a `Window`.");
        }); });
    }

    void Window::attach(Mouse* mouse)
    {
        insert_task([this,mouse]() { m_GraphicsThread.wait_task([this,mouse]()
        {
            if (m_MouseHandle != nullptr)
            {
                ROCKHOPPER_INTERNAL_LOG_ERROR("A `Mouse` is already attached to this `Window`!");
                return;
            }
            if (mouse->m_WindowHandle != nullptr)
            {
                ROCKHOPPER_INTERNAL_LOG_ERROR("A `Window` is already attached to this `Mouse`!");
                return;
            }
            m_MouseHandle = mouse;
            mouse->m_WindowHandle = this;
            GLFW_Context::SetMouseGLFWCallbacks<true>(m_WindowHandle);
            ROCKHOPPER_INTERNAL_LOG_INFO("Attached a `Mouse` to a `Window`.");
        }); });
    }

    void Window::detach(Mouse* mouse)
    {
        insert_task([this,mouse]() { m_GraphicsThread.wait_task([this,mouse]()
        {
            GLFW_Context::SetMouseGLFWCallbacks<false>(m_WindowHandle);
            mouse->m_WindowHandle = nullptr;
            m_MouseHandle = nullptr;
            ROCKHOPPER_INTERNAL_LOG_INFO("Detached a `Mouse` to a `Window`.");
        }); });
    }

    void Window::init()
    {
        m_GraphicsThread.wait_task([this]()
        {
            // Register a GLFW context
            GLFW_Context::Register();

            // Create a GLFW windowed-mode window handle and it's OpenGL context
            m_WindowHandle = glfwCreateWindow(m_Details.width,m_Details.height,m_Details.title.c_str(),NULL,NULL);
            ROCKHOPPER_INTERNAL_ASSERT_FATAL(m_WindowHandle,"Failed to create a GLFW window handle!");
            ROCKHOPPER_INTERNAL_LOG_DEBUG("Created a GLFW window.");

            // Set the GLFW user pointer to this window
            glfwSetWindowUserPointer(m_WindowHandle,this);

            // Set GLFW callbacks
            GLFW_Context::SetWindowGLFWCallbacks<true>(m_WindowHandle);

            // Make the window's context current
            glfwMakeContextCurrent(m_WindowHandle);

            // Register an OpenGL context
            OpenGL_Context::Register();

            // Dispatch a `WindowInitEvent` event
            {
                WindowInitEvent event;
                event.window = this;
                dispatch_event(event);
            }
        });
    }

    void Window::tick()
    {
        m_GraphicsThread.wait_task([this]()
        {
            if (glfwWindowShouldClose(m_WindowHandle))
            {
                stop();
            }

            // Poll window events
            if (m_KeyboardHandle) m_KeyboardHandle->tick();
            if (m_MouseHandle) m_MouseHandle->tick();
            glfwPollEvents();

            // Draw the last frame
            glfwSwapBuffers(m_WindowHandle);
            glClear(GL_COLOR_BUFFER_BIT);

            // Dispatch a `WindowRefreshEvent` event
            {
                WindowRefreshEvent event;
                event.window = this;
                dispatch_event(event);
            }
        });
    }

    void Window::dispose()
    {
        m_GraphicsThread.wait_task([this]()
        {
            // Dispatch a `WindowDisposeEvent` event
            {
                WindowDisposeEvent event;
                event.window = this;
                dispatch_event(event);
            }

            // Deregister an OpenGL context
            OpenGL_Context::Deregister();

            // Set GLFW callbacks
            GLFW_Context::SetWindowGLFWCallbacks<false>(m_WindowHandle);

            // Remove the GLFW user pointer
            glfwSetWindowUserPointer(m_WindowHandle,nullptr);

            // Destroy the GLFW window
            glfwDestroyWindow(m_WindowHandle);

            // Deregister an GLFW context
            GLFW_Context::Deregister();
        });
    }

} // namespace RockHopper

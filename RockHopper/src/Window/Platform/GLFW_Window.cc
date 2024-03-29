
#include "RockHopper/Window/Platform/GLFW_Window.hh"

#include "RockHopper/Debug.hh"
#include "RockHopper/Engine/Engine.hh"
#include "RockHopper/Input/Keyboard/Keyboard.hh"
#include "RockHopper/Input/Mouse/Mouse.hh"

#include <GLFW/glfw3.h>

namespace RockHopper
{

    GLFW_Window::~GLFW_Window()
    {
        // Unset GLFW callbacks
        m_GlfwContext.set_callbacks<Window,false>(&m_WindowHandle,(Window*)this);

        Renderer::GetInstance()->push_task([this]()
        {
            // Remove the GLFW user pointer
            glfwSetWindowUserPointer(m_WindowHandle,nullptr);

            // Destroy the GLFW window
            glfwDestroyWindow(m_WindowHandle);
        });

        // Stop the renderer
        Renderer::GetInstance()->stop();
        Renderer::GetInstance()->stop_notifier().wait();
    }

    GLFW_Window::GLFW_Window(Renderer::API renderer_api, WindowDetails const& details)
        : Window{renderer_api, details.title}
        , m_Details{details}
    {
        m_DebugName.set_type("GLFW_Window");
        set_details(details);

        // Start the renderer
        Renderer::GetInstance()->start();

        // Create the GLFW window
        Renderer::GetInstance()->push_task([this]()
        {
            // Create a GLFW windowed-mode window handle and it's OpenGL context
            m_WindowHandle = glfwCreateWindow(m_Details.width,m_Details.height,m_Details.title.c_str(),NULL,NULL);
            ROCKHOPPER_INTERNAL_ASSERT_FATAL(m_WindowHandle,"GLFW failed to create a handle for {}",m_DebugName);
            ROCKHOPPER_INTERNAL_LOG_DEBUG("GLFW {} created",m_DebugName);

            // Set the GLFW user pointer to this window
            glfwSetWindowUserPointer(m_WindowHandle,this);

            // Invoke window callbacks
            glfwSetWindowSize(m_WindowHandle,m_Details.width,m_Details.height);
        });

        // Set GLFW callbacks
        m_GlfwContext.set_callbacks<Window,true>(&m_WindowHandle,(Window*)this);

        // Make the window's context current
        Renderer::GetInstance()->wait_task([this]()
        {
            glfwMakeContextCurrent(m_WindowHandle);
        });
        // Don't return until everything is set up properly!
    }

    void GLFW_Window::set_details(WindowDetails const& details)
    {
        m_TaskQueue.push_task([this,details]() { Renderer::GetInstance()->push_task([this,details]()
        {
            ROCKHOPPER_INTERNAL_LOG_INFO("GLFW configuring {}", m_DebugName);
            if (m_WindowHandle)
            {
                glfwSetWindowSize(m_WindowHandle,details.width,details.height);
                glfwSetWindowTitle(m_WindowHandle,details.title.c_str());
            }
            m_Details = details;
        }); });
    }

    auto GLFW_Window::get_details() const -> WindowDetails const&
    {
        return m_Details;
    }

    void GLFW_Window::swap_interval(int interval)
    {
        m_TaskQueue.push_task([this,interval]() { Renderer::GetInstance()->push_task([this,interval]()
        {
            glfwMakeContextCurrent(m_WindowHandle);
            glfwSwapInterval(interval);
        }); });
    }

    void GLFW_Window::attach(Keyboard* keyboard)
    {
        m_TaskQueue.push_task([this,keyboard]() { Renderer::GetInstance()->push_task([this,keyboard]()
        {
            ROCKHOPPER_INTERNAL_LOG_INFO("attaching {} to {}", keyboard->m_DebugName, m_DebugName);
            if (m_KeyboardHandle != nullptr)
            {
                ROCKHOPPER_INTERNAL_LOG_ERROR("{} already attached to {}", keyboard->m_DebugName, m_DebugName);
                return;
            }
            m_KeyboardHandle = keyboard;
            keyboard->on_attach(this);

            m_GlfwContext.set_callbacks<Keyboard,true>(&m_WindowHandle,keyboard);
        }); });
    }

    void GLFW_Window::detach(Keyboard* keyboard)
    {
        m_TaskQueue.push_task([this,keyboard]() { Renderer::GetInstance()->push_task([this,keyboard]()
        {
            ROCKHOPPER_INTERNAL_LOG_INFO("detaching {} from {}", keyboard->m_DebugName, m_DebugName);
            m_KeyboardHandle = nullptr;
            keyboard->on_detach(this);

            m_GlfwContext.set_callbacks<Keyboard,false>(&m_WindowHandle,keyboard);
        }); });
    }

    void GLFW_Window::attach(Mouse* mouse)
    {
        m_TaskQueue.push_task([this,mouse]() { Renderer::GetInstance()->push_task([this,mouse]()
        {
            ROCKHOPPER_INTERNAL_LOG_INFO("attaching {} to {}", mouse->m_DebugName, m_DebugName);
            if (m_MouseHandle != nullptr)
            {
                ROCKHOPPER_INTERNAL_LOG_ERROR("{} already attached to {}", mouse->m_DebugName, m_DebugName);
                return;
            }
            m_MouseHandle = mouse;
            mouse->on_attach(this);

            m_GlfwContext.set_callbacks<Mouse,true>(&m_WindowHandle,mouse);
        }); });
    }

    void GLFW_Window::detach(Mouse* mouse)
    {
        m_TaskQueue.push_task([this,mouse]() { Renderer::GetInstance()->push_task([this,mouse]()
        {
            ROCKHOPPER_INTERNAL_LOG_INFO("detaching {} from {}", mouse->m_DebugName, m_DebugName);
            m_MouseHandle = nullptr;
            mouse->on_detach(this);

            m_GlfwContext.set_callbacks<Mouse,false>(&m_WindowHandle,mouse);
        }); });
    }

    void GLFW_Window::on_event(EngineInitEvent const&)
    {
        Renderer::GetInstance()->wait_task([this]()
        {
            WindowInitEvent event;
            event.window = this;
            dispatch_event(event);
        });
    }

    void GLFW_Window::on_event(EngineDisposeEvent const&)
    {
        Renderer::GetInstance()->wait_task([this]()
        {
            WindowDisposeEvent event;
            event.window = this;
            dispatch_event(event);
        });
    }

    void GLFW_Window::on_event(EngineTickEvent const& event)
    {
        if (glfwWindowShouldClose(m_WindowHandle))
        {
            event.engine->stop();
        }

        m_TaskQueue.execute_all();

        if (m_KeyboardHandle) m_KeyboardHandle->tick();
        if (m_MouseHandle) m_MouseHandle->tick();

        Renderer::GetInstance()->push_task([this]()
        {
            glfwPollEvents();
            glfwSwapBuffers(m_WindowHandle);
        });

        Renderer::GetInstance()->wait_task([this]()
        {
            Renderer::GetInstance()->scene_begin(m_Camera.get());

            WindowRefreshEvent event;
            event.window = this;
            dispatch_event(event);

            Renderer::GetInstance()->scene_end();
        });
    }

} // namespace RockHopper

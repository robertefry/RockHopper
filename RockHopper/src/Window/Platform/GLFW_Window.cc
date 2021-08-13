
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

        Renderer::GetInstance()->push_task([this]()
        {
            // Create a GLFW windowed-mode window handle and it's OpenGL context
            m_WindowHandle = glfwCreateWindow(1,1,m_Details.title.c_str(),NULL,NULL);
            ROCKHOPPER_INTERNAL_ASSERT_FATAL(m_WindowHandle,"Failed to create a GLFW window handle! {}", m_DebugName);
            ROCKHOPPER_INTERNAL_LOG_DEBUG("Created a GLFW window {}.", m_DebugName);

            // Set the GLFW user pointer to this window
            glfwSetWindowUserPointer(m_WindowHandle,this);

            // Set window properties via callbacks
            glfwSetWindowSize(m_WindowHandle,m_Details.width,m_Details.height);
        });

        // Set GLFW callbacks
        m_GlfwContext.set_callbacks<Window,true>(&m_WindowHandle,(Window*)this);

        Renderer::GetInstance()->push_task([this]()
        {
            // Make the window's context current
            glfwMakeContextCurrent(m_WindowHandle);
        });

        // Start the renderer
        Renderer::GetInstance()->start();
    }

    void GLFW_Window::set_details(WindowDetails const& details)
    {
        m_TaskQueue.push_task([this,details]() { Renderer::GetInstance()->push_task([this,details]()
        {
            ROCKHOPPER_INTERNAL_LOG_INFO("Setting details for {}.", m_DebugName);
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

    void GLFW_Window::attach(Keyboard* keyboard)
    {
        m_TaskQueue.push_task([this,keyboard]() { Renderer::GetInstance()->push_task([this,keyboard]()
        {
            ROCKHOPPER_INTERNAL_LOG_INFO("Attaching the Keyboard {} to the Window {}.", keyboard->m_DebugName, m_DebugName);
            if (m_KeyboardHandle != nullptr)
            {
                ROCKHOPPER_INTERNAL_LOG_ERROR("The Keyboard {} is already attached to the Window {}!", keyboard->m_DebugName, m_DebugName);
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
            ROCKHOPPER_INTERNAL_LOG_INFO("Detaching the Keyboard {} from the Window {}.", keyboard->m_DebugName, m_DebugName);
            m_KeyboardHandle = nullptr;
            keyboard->on_detach(this);

            m_GlfwContext.set_callbacks<Keyboard,false>(&m_WindowHandle,keyboard);
        }); });
    }

    void GLFW_Window::attach(Mouse* mouse)
    {
        m_TaskQueue.push_task([this,mouse]() { Renderer::GetInstance()->push_task([this,mouse]()
        {
            ROCKHOPPER_INTERNAL_LOG_INFO("Attaching the Mouse {} to the Window {}.", mouse->m_DebugName, m_DebugName);
            if (m_MouseHandle != nullptr)
            {
                ROCKHOPPER_INTERNAL_LOG_ERROR("The Mouse {} is already attached to the Window {}!", mouse->m_DebugName, m_DebugName);
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
            ROCKHOPPER_INTERNAL_LOG_INFO("Detached the Mouse {} from the Window {}.", mouse->m_DebugName, m_DebugName);
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
        Renderer::GetInstance()->refresh();

        Renderer::GetInstance()->wait_task([this]()
        {
            WindowRefreshEvent event;
            event.window = this;
            dispatch_event(event);
        });
    }

} // namespace RockHopper

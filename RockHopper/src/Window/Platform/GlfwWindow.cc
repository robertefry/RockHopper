
#include "RockHopper/Window/Platform/GlfwWindow.hh"

#include "RockHopper/Debug.hh"
#include "RockHopper/Input/Keyboard/Keyboard.hh"
#include "RockHopper/Input/Mouse/Mouse.hh"

#include <GLFW/glfw3.h>

namespace RockHopper
{

    GlfwWindow::~GlfwWindow()
    {
        m_RenderThread.wait_task([this]()
        {
            // Dispose the render context
            m_Renderer->dispose();
        });

        // Unset GLFW callbacks
        m_GlfwContext.set_callbacks<Window,false>(m_WindowHandle,(Window*)this);

        m_RenderThread.wait_task([this]()
        {
            // Remove the GLFW user pointer
            glfwSetWindowUserPointer(m_WindowHandle,nullptr);

            // Destroy the GLFW window
            glfwDestroyWindow(m_WindowHandle);
        });
    }

    GlfwWindow::GlfwWindow(Renderer::API render_api, WindowDetails const& details)
        : Window{details.title}
        , m_Renderer{Renderer::Create(render_api)}
        , m_Details{details}
    {
        m_DebugName.set_type("GlfwWindow");
        set_details(details);

        m_RenderThread.wait_task([this]()
        {
            // Create a GLFW windowed-mode window handle and it's OpenGL context
            m_WindowHandle = glfwCreateWindow(m_Details.width,m_Details.height,m_Details.title.c_str(),NULL,NULL);
            ROCKHOPPER_INTERNAL_ASSERT_FATAL(m_WindowHandle,"Failed to create a GLFW window handle! {}", m_DebugName);
            ROCKHOPPER_INTERNAL_LOG_DEBUG("Created a GLFW window {}.", m_DebugName);

            // Set the GLFW user pointer to this window
            glfwSetWindowUserPointer(m_WindowHandle,this);
        });

        // Set GLFW callbacks
        m_GlfwContext.set_callbacks<Window,true>(m_WindowHandle,(Window*)this);

        m_RenderThread.wait_task([this]()
        {
            // Make the window's context current
            glfwMakeContextCurrent(m_WindowHandle);

            // Initialize the render context
            m_Renderer->initialize();
        });
    }

    void GlfwWindow::set_details(WindowDetails const& details)
    {
        insert_task([this,details]() { m_RenderThread.push_task([this,details]()
        {
            m_Timing.set_omega(details.frametime);

            ROCKHOPPER_INTERNAL_LOG_INFO("Setting details for {}.", m_DebugName);
            if (m_WindowHandle)
            {
                glfwSetWindowSize(m_WindowHandle,details.width,details.height);
                glfwSetWindowTitle(m_WindowHandle,details.title.c_str());
            }
            m_Details = details;
        }); });
    }

    auto GlfwWindow::get_details() const -> WindowDetails const&
    {
        return m_Details;
    }

    void GlfwWindow::attach(Keyboard* keyboard)
    {
        insert_task([this,keyboard]() { m_RenderThread.wait_task([this,keyboard]()
        {
            ROCKHOPPER_INTERNAL_LOG_INFO("Attaching the Keyboard {} to the Window {}.", keyboard->m_DebugName, m_DebugName);
            if (m_KeyboardHandle != nullptr)
            {
                ROCKHOPPER_INTERNAL_LOG_ERROR("The Keyboard {} is already attached to the Window {}!", keyboard->m_DebugName, m_DebugName);
                return;
            }
            m_KeyboardHandle = keyboard;
            keyboard->on_attach(this);

            m_GlfwContext.set_callbacks<Keyboard,true>(m_WindowHandle,keyboard);
        }); });
    }

    void GlfwWindow::detach(Keyboard* keyboard)
    {
        insert_task([this,keyboard]() { m_RenderThread.wait_task([this,keyboard]()
        {
            ROCKHOPPER_INTERNAL_LOG_INFO("Detaching the Keyboard {} from the Window {}.", keyboard->m_DebugName, m_DebugName);
            m_KeyboardHandle = nullptr;
            keyboard->on_detach(this);

            m_GlfwContext.set_callbacks<Keyboard,false>(m_WindowHandle,keyboard);
        }); });
    }

    void GlfwWindow::attach(Mouse* mouse)
    {
        insert_task([this,mouse]() { m_RenderThread.wait_task([this,mouse]()
        {
            ROCKHOPPER_INTERNAL_LOG_INFO("Attaching the Mouse {} to the Window {}.", mouse->m_DebugName, m_DebugName);
            if (m_MouseHandle != nullptr)
            {
                ROCKHOPPER_INTERNAL_LOG_ERROR("The Mouse {} is already attached to the Window {}!", mouse->m_DebugName, m_DebugName);
                return;
            }
            m_MouseHandle = mouse;
            mouse->on_attach(this);

            m_GlfwContext.set_callbacks<Mouse,true>(m_WindowHandle,mouse);
        }); });
    }

    void GlfwWindow::detach(Mouse* mouse)
    {
        insert_task([this,mouse]() { m_RenderThread.wait_task([this,mouse]()
        {
            ROCKHOPPER_INTERNAL_LOG_INFO("Detached the Mouse {} from the Window {}.", mouse->m_DebugName, m_DebugName);
            m_MouseHandle = nullptr;
            mouse->on_detach(this);

            m_GlfwContext.set_callbacks<Mouse,false>(m_WindowHandle,mouse);
        }); });
    }

    void GlfwWindow::init()
    {
        m_RenderThread.wait_task([this]()
        {
            WindowInitEvent event;
            event.window = this;
            dispatch_event(event);
        });
    }

    void GlfwWindow::tick()
    {
        if (glfwWindowShouldClose(m_WindowHandle))
        {
            stop();
        }

        if (m_KeyboardHandle) m_KeyboardHandle->tick();
        if (m_MouseHandle) m_MouseHandle->tick();

        m_RenderThread.wait_task([this]()
        {
            glfwPollEvents();

            glfwSwapBuffers(m_WindowHandle);
            m_Renderer->refresh();
        });

        m_RenderThread.wait_task([this]()
        {
            WindowRefreshEvent event;
            event.window = this;
            dispatch_event(event);
        });

        // Wait on a blank graphics task to synchronize the graphics thread
        // with the current window thread.
        m_RenderThread.wait_task([](){});
    }

    void GlfwWindow::dispose()
    {
        m_RenderThread.wait_task([this]()
        {
            WindowDisposeEvent event;
            event.window = this;
            dispatch_event(event);
        });
    }

} // namespace RockHopper

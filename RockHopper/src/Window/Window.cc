
#include "RockHopper/Window/Window.hh"

#include "RockHopper/Debug.hh"
#include "RockHopper/Input/Keyboard/Keyboard.hh"
#include "RockHopper/Input/Mouse/Mouse.hh"

#include "GLFW/GLFW_Context.hh"

#include <GLFW/glfw3.h>

#include <utility>

namespace RockHopper
{

    Window::Window(WindowDetails const& details)
        : m_WindowHandle{}
        , m_Details{details}
    {
        // Initialise GLFW
        GLFW_Context::Register();

        // Create a GLFW windowed-mode window handle and it's OpenGL context
        m_WindowHandle = glfwCreateWindow(m_Details.width,m_Details.height,m_Details.title.c_str(),NULL,NULL);
        ROCKHOPPER_INTERNAL_ASSERT_FATAL(m_WindowHandle,"Failed to create a GLFW window handle!");

        // Set the GLFW user pointer to this window
        glfwSetWindowUserPointer(m_WindowHandle,this);
    }

    Window::~Window()
    {
        // Remove the GLFW user pointer
        glfwSetWindowUserPointer(m_WindowHandle,nullptr);

        // Destroy the GLFW window
        glfwDestroyWindow(m_WindowHandle);

        // Uninitialize the GLFW context
        GLFW_Context::Deregister();
    }

    void Window::set_details(WindowDetails const& details)
    {
        m_Details = details;

        ROCKHOPPER_INTERNAL_LOG_INFO("Setting details for a window.");
        if (m_WindowHandle)
        {
            glfwSetWindowSize(m_WindowHandle,details.width,details.height);
            glfwSetWindowTitle(m_WindowHandle,details.title.c_str());
        }
        m_Timing.set_omega(details.frametime);
    }

    auto Window::get_details() const -> WindowDetails const&
    {
        return m_Details;
    }

    void Window::attach(Keyboard* keyboard)
    {
        if (m_KeyboardHandle != nullptr)
        {
            ROCKHOPPER_INTERNAL_LOG_ERROR("Window already has a keyboard attached!");
            return;
        }
        m_KeyboardHandle = keyboard;
        keyboard->m_WindowHandle = this;
        SetKeyboardGLFWCallbacks<true>(m_WindowHandle);
    }

    void Window::detach(Keyboard* keyboard)
    {
        SetKeyboardGLFWCallbacks<false>(m_WindowHandle);
        keyboard->m_WindowHandle = nullptr;
        m_KeyboardHandle = nullptr;
    }

    void Window::attach(Mouse* mouse)
    {
        if (m_MouseHandle != nullptr)
        {
            ROCKHOPPER_INTERNAL_LOG_ERROR("Window already has a mouse attached!");
            return;
        }
        m_MouseHandle = mouse;
        mouse->m_WindowHandle = this;
        SetMouseGLFWCallbacks<true>(m_WindowHandle);
    }

    void Window::detach(Mouse* mouse)
    {
        SetMouseGLFWCallbacks<false>(m_WindowHandle);
        mouse->m_WindowHandle = nullptr;
        m_MouseHandle = nullptr;
    }

    void Window::init()
    {
        // Set GLFW callbacks
        SetWindowGLFWCallbacks<true>(m_WindowHandle);

        // Make the window's context current
        glfwMakeContextCurrent(m_WindowHandle);
    }

    void Window::tick()
    {
        if (glfwWindowShouldClose(m_WindowHandle))
        {
            stop();
        }

        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(m_WindowHandle);

        if (m_KeyboardHandle) m_KeyboardHandle->tick();
        glfwPollEvents();
    }

    void Window::dispose()
    {
        // Set GLFW callbacks
        SetWindowGLFWCallbacks<false>(m_WindowHandle);
    }

} // namespace RockHopper

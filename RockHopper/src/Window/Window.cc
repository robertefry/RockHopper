
#include "RockHopper/Window/Window.hh"

#include "RockHopper/Debug.hh"

#include "GLFW_Context.hh"

#include <utility>

namespace RockHopper
{

    Window::Window(WindowDetails const& details)
        : m_WindowHandle{}
        , m_Details{details}
    {
    }

    Window::~Window()
    {
    }

    Window::Window(Window&& other)
        : m_WindowHandle{}
        , m_Details{other.m_Details}
    {
        std::swap(m_WindowHandle,other.m_WindowHandle);
    }

    Window& Window::operator=(Window&& other)
    {
        std::swap(m_WindowHandle,other.m_WindowHandle);
        std::swap(m_Details,other.m_Details);
        return *this;
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

    void Window::init()
    {
        // Initialise GLFW
        GLFW_Context::Register();

        // Set the window details
        set_details(m_Details);

        // Create a GLFW windowed-mode window handle and it's OpenGL context
        m_WindowHandle = glfwCreateWindow(m_Details.width,m_Details.height,m_Details.title.c_str(),NULL,NULL);
        ROCKHOPPER_INTERNAL_ASSERT_FATAL(m_WindowHandle,"Failed to create a GLFW window handle!");

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
        glfwPollEvents();
    }

    void Window::dispose()
    {
        GLFW_Context::Deregister();
    }

} // namespace RockHopper

/**
 * @author Robert Fry
 * @date create 20-Jun-2021
 * @date modify 20-Jun-2021
 */

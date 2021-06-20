
#include "RockHopper/Window/Window.hh"

#include "RockHopper/Logging/Logger.hh"

namespace RockHopper
{

    Window::Window(WindowDetails const& details)
        : m_Details{details}
    {
        m_Timing.set_omega(details.frametime);
    }

    Window::~Window()
    {
    }

    void Window::init()
    {
        // Initialize GLFW
        if (!glfwInit())
        {
            ROCKHOPPER_LOGINTERNAL_FATAL("Failed to initialize GLFW!");
            abort();
        }

        // Create a GLFW windowed-mode window handle and it's OpenGL context
        m_WindowHandle = glfwCreateWindow(m_Details.width,m_Details.height,
            m_Details.title.c_str(),NULL,NULL);
        if (!m_WindowHandle)
        {
            ROCKHOPPER_LOGINTERNAL_FATAL("Failed to create a GLFW window handle!");
            abort();
        }

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
        glfwTerminate();
    }

} // namespace RockHopper

/**
 * @author Robert Fry
 * @date create 20-Jun-2021
 * @date modify 20-Jun-2021
 */

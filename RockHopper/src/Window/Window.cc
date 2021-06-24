
#include "RockHopper/Window/Window.hh"

#include "RockHopper/Window/WindowEvents.hh"
#include "RockHopper/Debug.hh"

#include "GLFW_Context.hh"

#include <GLFW/glfw3.h>

#include <utility>

void SetWindowGLFWCallbacks(GLFWwindow* handle);

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

        // Set the GLFW user pointer to this window
        glfwSetWindowUserPointer(m_WindowHandle,this);

        // Set GLFW callbacks
        SetWindowGLFWCallbacks(m_WindowHandle);

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
        glfwDestroyWindow(m_WindowHandle);
        GLFW_Context::Deregister();
    }

} // namespace RockHopper

void SetWindowGLFWCallbacks(GLFWwindow* handle)
{
    using namespace RockHopper;

    glfwSetWindowPosCallback(handle,[](GLFWwindow* handle, int x, int y)
    {
        WindowPositionEvent event;
        event.x = x;
        event.y = y;

        Window* window = (Window*)glfwGetWindowUserPointer(handle);
        event.window = window;
        window->WindowEventHandler::dispatch_event(event);
    });
    glfwSetWindowSizeCallback(handle,[](GLFWwindow* handle, int width, int height)
    {
        WindowSizeEvent event;
        event.width = width;
        event.height = height;

        Window* window = (Window*)glfwGetWindowUserPointer(handle);
        event.window = window;
        window->WindowEventHandler::dispatch_event(event);
    });
    glfwSetWindowCloseCallback(handle,[](GLFWwindow* handle)
    {
        WindowCloseEvent event;

        Window* window = (Window*)glfwGetWindowUserPointer(handle);
        event.window = window;
        window->WindowEventHandler::dispatch_event(event);
    });
    glfwSetWindowRefreshCallback(handle,[](GLFWwindow* handle)
    {
        WindowRefreshEvent event;

        Window* window = (Window*)glfwGetWindowUserPointer(handle);
        event.window = window;
        window->WindowEventHandler::dispatch_event(event);
    });
    glfwSetWindowFocusCallback(handle,[](GLFWwindow* handle, int focus)
    {
        WindowFocusEvent event;
        event.focus = focus;

        Window* window = (Window*)glfwGetWindowUserPointer(handle);
        event.window = window;
        window->WindowEventHandler::dispatch_event(event);
    });
    glfwSetWindowIconifyCallback(handle,[](GLFWwindow* handle, int iconified)
    {
        WindowMinimizeEvent event;
        event.minimized = iconified;

        Window* window = (Window*)glfwGetWindowUserPointer(handle);
        event.window = window;
        window->WindowEventHandler::dispatch_event(event);
    });
    glfwSetWindowMaximizeCallback(handle,[](GLFWwindow* handle, int maximized)
    {
        WindowMaximizedEvent event;
        event.maximized = maximized;

        Window* window = (Window*)glfwGetWindowUserPointer(handle);
        event.window = window;
        window->WindowEventHandler::dispatch_event(event);
    });
    glfwSetWindowContentScaleCallback(handle,[](GLFWwindow* handle, float scale_x, float scale_y)
    {
        WindowScaleEvent event;
        event.scale_x = scale_x;
        event.scale_y = scale_y;

        Window* window = (Window*)glfwGetWindowUserPointer(handle);
        event.window = window;
        window->WindowEventHandler::dispatch_event(event);
    });
    glfwSetFramebufferSizeCallback(handle,[](GLFWwindow* handle, int width, int height)
    {
        WindowBufferSizeEvent event;
        event.width = width;
        event.height = height;

        Window* window = (Window*)glfwGetWindowUserPointer(handle);
        event.window = window;
        window->WindowEventHandler::dispatch_event(event);
    });
}

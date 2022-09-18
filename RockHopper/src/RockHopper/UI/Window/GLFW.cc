
#include "RockHopper/UI/Window/GLFW.hh"
#include "RockHopper/UI/Window/GLFW_Context.hh"

#include "RockHopper/Debug.hh"

#include <GLFW/glfw3.h>

/* ************************************************************************** */
// RockHopper::UI::GLFW::Window::Impl
/* ************************************************************************** */

namespace RockHopper::UI::GLFW
{

    struct Window::Impl
    {
        GLFWwindow* window = nullptr;

        width_t width = 1;
        height_t height = 1;
        title_t title = "";
        swap_interval_t swap_interval = 0;

        GLFW_Context context;

        /*
         * requires the identity;
         *  ∀ GLFW::Window* window != nullptr; window->m_Impl->parent == window
         */
        GLFW::Window* parent = nullptr;

    public:
        virtual ~Impl() = default;

        explicit Impl(
            std::shared_ptr<GX::Renderer> const& renderer
        )
            : context{renderer}
        {}

        Impl(Impl const&) = delete;
        Impl& operator=(Impl const&) = delete;

        Impl(Impl&&) = default;
        Impl& operator=(Impl&&) = default;

    public:
        void create_window();
        void destroy_window();
        void link_window();
    };

    void Window::Impl::create_window()
    {
        // Create a GLFW windowed-mode window handle and it's OpenGL context
        window = glfwCreateWindow((int)width,(int)height,title.c_str(),nullptr,nullptr);
        ROCKHOPPER_INTERNAL_ASSERT_FATAL(window,"GLFW failed to create a window");
        ROCKHOPPER_INTERNAL_LOG_DEBUG("GLFW created a window {}",(void const*)window);

        link_window();

        // Set swap interval
        glfwMakeContextCurrent(window);
        glfwSwapInterval(swap_interval);

        // Dispatch a window-creation event
        {
            WindowEvent_Create event;
            event.window = parent;
            parent->dispatch_event(event);
        }
    }

    void Window::Impl::destroy_window()
    {
        // Destroy the GLFW window
        glfwDestroyWindow(window);

        window = nullptr;

        // Dispatch a window-destruction event
        {
            WindowEvent_Destroy event;
            event.window = parent;
            parent->dispatch_event(event);
        }
    }

    void Window::Impl::link_window()
    {
        // Set the GLFW user pointer to this window
        glfwSetWindowUserPointer(window,parent);
    }

} // namespace RockHopper::UI::GLFW

/* ************************************************************************** */
// [Implementation] RockHopper::UI::GLFW::Window
/* ************************************************************************** */

namespace RockHopper::UI::GLFW
{

    Window::~Window()
    {
        m_Renderer->wait_task([this]()
        {
            if (m_Impl->window) m_Impl->destroy_window();
        });
    }

    Window::Window(
        std::shared_ptr<Chrono::I_TickThread> const& tick_thread,
        std::shared_ptr<GX::Renderer> const& renderer
    )
        : UI::Window{tick_thread,renderer}
        , m_Impl{std::make_unique<Impl>(renderer)}
    {
        m_Impl->parent = this; // fix identity
    }

    Window::Window(Window&& other)
        : UI::Window{std::move(other)}
        , m_Impl{std::move(other.m_Impl)}
    {
        m_Impl->parent = this; // fix identity
    }

    Window& Window::operator=(Window&& other)
    {
        UI::Window::operator=(std::move(other));
        m_Impl = std::move(other.m_Impl);

        m_Impl->parent = this; // fix identity
        return *this;
    }

    void Window::on_event(WindowEvent_Init const&)
    {
        // TODO: Window::on_event(WindowEvent_Init const&)
    }

    void Window::on_event(WindowEvent_Dispose const& event)
    {
        // TODO: Window::on_event(WindowEvent_Dispose const&)
    }

    void Window::on_event(WindowEvent_Refresh const&)
    {
        // TODO: Window::on_event(WindowEvent_Refresh const&)

        // m_Renderer->wait_task([this]()
        // {
        //     if (m_Impl->window)
        //     {
        //         glfwPollEvents();
        //         glfwSwapBuffers(m_Impl->window);

        //         if (glfwWindowShouldClose(m_Impl->window)) {
        //             m_Impl->destroy_window();
        //         }
        //     }
        // });
    }

    bool Window::is_visible() const
    {
        return m_Impl->window != nullptr;
    }

    auto Window::show() -> std::future<void>
    {
        return m_Renderer->push_task([this]()
        {
            if (not m_Impl->window) m_Impl->create_window();
        });
    }

    auto Window::hide() -> std::future<void>
    {
        return m_Renderer->push_task([this]()
        {
            if (m_Impl->window) m_Impl->destroy_window();
        });
    }

    auto Window::get_width() const -> width_t
    {
        return m_Impl->width;
    }

    auto Window::set_width(width_t const& width) -> std::future<void>
    {
        m_Impl->width = width;

        return m_Renderer->push_task([this]()
        {
            if (m_Impl->window) {
                glfwSetWindowSize(m_Impl->window,m_Impl->width,m_Impl->height);
            }
        });
    }

    auto Window::get_height() const -> height_t
    {
        return m_Impl->height;
    }

    auto Window::set_height(height_t const& height) -> std::future<void>
    {
        m_Impl->height = height;

        return m_Renderer->push_task([this]()
        {
            if (m_Impl->window) {
                glfwSetWindowSize(m_Impl->window,m_Impl->width,m_Impl->height);
            }
        });
    }

    auto Window::get_title() const -> title_t
    {
        return m_Impl->title;
    }

    auto Window::set_title(title_t const& title) -> std::future<void>
    {
        m_Impl->title = title;

        return m_Renderer->push_task([this]()
        {
            if (m_Impl->window) {
                glfwSetWindowTitle(m_Impl->window,m_Impl->title.c_str());
            }
        });
    }

    auto Window::get_swap_interval() const -> swap_interval_t
    {
        return m_Impl->swap_interval;
    }

    auto Window::set_swap_interval(swap_interval_t const& swap_interval) -> std::future<void>
    {
        m_Impl->swap_interval = swap_interval;

        return m_Renderer->push_task([this]()
        {
            if (m_Impl->window) {
                glfwMakeContextCurrent(m_Impl->window);
                glfwSwapInterval(m_Impl->swap_interval);
            }
        });
    }

} // namespace RockHopper::UI::GLFW

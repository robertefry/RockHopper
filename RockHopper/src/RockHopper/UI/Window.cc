
#include "RockHopper/UI/Window.hh"

/* ************************************************************************** */
// RockHopper::UI::Window::TickListener
/* ************************************************************************** */

namespace RockHopper::UI
{

    /*
     *  requires the following identity always hold true
     *      ∀ Window* window != nullptr. window->m_TickListener->m_Window == window
     *  scoped private to fully control this identity.
     */
    class Window::TickListener
        : public Chrono::TickEvent::ListenerType
    {
        friend Window;

    public:
        virtual ~TickListener()
        {}
        explicit TickListener(Window* window)
            : m_Window{window}
        {}

        TickListener(TickListener const& other) = default;
        TickListener& operator=(TickListener const&) = default;

    private:
        /*
         * Tick events are translated to Window events, and are then dispatched
         * off the renderer thread so that no deadlocks are caused whenever an
         * event should then dispatch a task to the renderer thread.
         */
        virtual void on_event(Chrono::TickEvent_Init const&) override
        {
            WindowEvent_Init event;
            event.window = m_Window;
            m_Window->dispatch_event(event);
        }
        virtual void on_event(Chrono::TickEvent_Dispose const&) override
        {
            WindowEvent_Dispose event;
            event.window = m_Window;
            m_Window->dispatch_event(event);
        }
        virtual void on_event(Chrono::TickEvent_OnTick const&) override
        {
            WindowEvent_Refresh event;
            event.window = m_Window;
            m_Window->dispatch_event(event);
        }

    private:
        Window* m_Window;
    };

} // namespace RockHopper::UI

/* ************************************************************************** */
// [Implementation] RockHopper::UI::Window
/* ************************************************************************** */

namespace RockHopper::UI
{

    Window::~Window()
    {
        if (m_TickThread && m_TickListener) {
            m_TickThread->remove_event_listener(m_TickListener);
        }
    }

    Window::Window(
        std::shared_ptr<Chrono::I_TickThread> const& tick_thread,
        std::shared_ptr<GX::Renderer> const& renderer
    )
        : EventHandler{}
        , m_TickThread{tick_thread}
        , m_Renderer{renderer}
        , m_TickListener{std::make_shared<TickListener>(this)}
    {
        m_TickThread->insert_event_listener(m_TickListener);
    }

    Window::Window(Window&& other)
        : EventHandler{std::move(other)}
        , m_TickThread{std::move(other.m_TickThread)}
        , m_Renderer{std::move(other.m_Renderer)}
        , m_TickListener{std::move(other.m_TickListener)}
    {
        m_TickListener->m_Window = this; // fix identity
    }

    Window& Window::operator=(Window&& other)
    {
        EventHandler::operator=(std::move(other));
        m_TickThread = std::move(other.m_TickThread);
        m_Renderer = std::move(other.m_Renderer);
        m_TickListener = std::move(other.m_TickListener);

        m_TickListener->m_Window = this; //fix identity
        return *this;
    }

    auto Window::renderer() const -> std::shared_ptr<GX::Renderer const>
    {
        return m_Renderer;
    }

    auto Window::renderer() -> std::shared_ptr<GX::Renderer>
    {
        return m_Renderer;
    }

} // namespace RockHopper::UI

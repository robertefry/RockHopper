
#ifndef ROCKHOPPER_UI_WINDOW_HH
#define ROCKHOPPER_UI_WINDOW_HH

#include "RockHopper/UI/Window.Events.fwd"

#include "RockHopper/Event/EventHandler.hh"
#include "RockHopper/Event/EventDispatch.Sequential.hh"
#include "RockHopper/Event/EventDispatch.ThreadPool.hh"

#include "RockHopper/Chrono/TickThread.hh"

#include "RockHopper/Util/ArgTraits.hh"
#include "RockHopper/Util/Future.hh"

namespace RockHopper::UI
{

    class Window
        : public Chrono::TickThread<Event::Dispatch::Sequential>
        , public Event::EventHandler<WindowEvent,Event::Dispatch::ThreadPool>
    {
        using EventHandler = Event::EventHandler<WindowEvent,Event::Dispatch::ThreadPool>;
        using TickThread = Chrono::TickThread<Event::Dispatch::Sequential>;

    public:
        virtual ~Window() = default;
        explicit Window() = default;

        Window(Window const&) = delete;
        Window& operator=(Window const&) = delete;

    public:

        enum visible_t { CLOSED, TRAY, MINIMISED, VISIBLE, MAXIMISED };
        [[nodiscard]] virtual auto get_visible() const -> visible_t = 0;
        [[nodiscard]] virtual auto set_visible(Util::In<visible_t>) -> Util::Future<void> = 0;

        enum focused_t { UNFOCUSED, FOCUSED };
        [[nodiscard]] virtual auto get_focused() const -> focused_t = 0;
        [[nodiscard]] virtual auto set_focused(Util::In<focused_t>) -> Util::Future<void> = 0;

        struct position_t { int32_t x, y; };
        [[nodiscard]] virtual auto get_position() const -> position_t = 0;
        [[nodiscard]] virtual auto set_position(Util::In<position_t>) -> Util::Future<void> = 0;

        struct dimension_t { uint32_t wid, hei; };
        [[nodiscard]] virtual auto get_dimension() const -> dimension_t = 0;
        [[nodiscard]] virtual auto set_dimension(Util::In<dimension_t>) -> Util::Future<void> = 0;

        using title_t = std::string;
        [[nodiscard]] virtual auto get_title() const -> title_t = 0;
        [[nodiscard]] virtual auto set_title(Util::In<title_t>) -> Util::Future<void> = 0;

        using swap_interval_t = uint32_t;
        [[nodiscard]] virtual auto get_swap_interval() const -> swap_interval_t = 0;
        [[nodiscard]] virtual auto set_swap_interval(Util::In<swap_interval_t>) -> Util::Future<void> = 0;

    protected:
        using EventHandler::dispatch_event;

        using EventHandler::on_event;

    private:
        void on_event(Chrono::TickEvent_Init const&) final;
        void on_event(Chrono::TickEvent_Dispose const&) final;
        void on_event(Chrono::TickEvent_OnTick const&) final;

    };

} // namespace RockHopper::UI

// window events use typedefs in the window class
#include "RockHopper/UI/Window.Events.hh"

#endif /* ROCKHOPPER_UI_WINDOW_HH */

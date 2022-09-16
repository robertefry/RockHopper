
#ifndef __HH_ROCKHOPPER_UI_WINDOW_
#define __HH_ROCKHOPPER_UI_WINDOW_

#include "RockHopper/UI/WindowEvents.hh"

#include "RockHopper/Event/EventHandler.hh"
#include "RockHopper/GX/Renderer.hh"

/* ************************************************************************** */
// [Definition] RockHopper::UI::I_Window
/* ************************************************************************** */

namespace RockHopper::UI
{

    struct I_Window
        : public virtual Event::I_EventHandler<WindowEvent>
    {
        virtual bool is_visible() const = 0;
        virtual auto show() -> std::future<void> = 0;
        virtual auto hide() -> std::future<void> = 0;

        virtual auto renderer() const -> std::shared_ptr<GX::Renderer const> = 0;
        virtual auto renderer() -> std::shared_ptr<GX::Renderer> = 0;

        using width_t = unsigned int;
        virtual auto get_width() const -> width_t = 0;
        virtual auto set_width(width_t const&) -> std::future<void> = 0;

        using height_t = unsigned int;
        virtual auto get_height() const -> height_t = 0;
        virtual auto set_height(height_t const&) -> std::future<void> = 0;

        using title_t = std::string;
        virtual auto get_title() const -> title_t = 0;
        virtual auto set_title(title_t const&) -> std::future<void> = 0;

        using swap_interval_t = unsigned int;
        virtual auto get_swap_interval() const -> swap_interval_t = 0;
        virtual auto set_swap_interval(swap_interval_t const&) -> std::future<void> = 0;
    };

} // namespace RockHopper::UI

/* ************************************************************************** */
// [Definition] RockHopper::UI::Window
/* ************************************************************************** */

namespace RockHopper::UI
{

    class Window
        : public virtual I_Window
        , public virtual Event::EventHandler<WindowEvent,Event::Dispatch::SequentialDispatch>
    {
    public:
        virtual ~Window();

        explicit Window(
            std::shared_ptr<Chrono::I_TickThread> const&,
            std::shared_ptr<GX::Renderer> const& );

        Window(Window&&);
        Window& operator=(Window&&);

    public:
        virtual auto renderer() const -> std::shared_ptr<GX::Renderer const> override;
        virtual auto renderer() -> std::shared_ptr<GX::Renderer> override;

    protected:
        std::shared_ptr<Chrono::I_TickThread> m_TickThread;
        std::shared_ptr<GX::Renderer> m_Renderer;

    private:
        class TickListener;
        std::shared_ptr<TickListener> m_TickListener;
    };

} // namespace RockHopper::UI

#endif /* __HH_ROCKHOPPER_UI_WINDOW_ */

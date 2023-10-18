
#ifndef ROCKHOPPER_UI_WINDOW_GLFW_HH
#define ROCKHOPPER_UI_WINDOW_GLFW_HH

#include "RockHopper/UI/Window.hh"

#include <memory>

namespace RockHopper::UI::GLFW
{

    class Window : public UI::Window
    {
    public:
        virtual ~Window();
        explicit Window();

        Window(Window const&) = delete;
        Window& operator=(Window const&) = delete;

        [[nodiscard]] auto get_visible() const -> visible_t override;
        [[nodiscard]] auto set_visible(Util::In<visible_t>) -> Util::Future<void> override;

        [[nodiscard]] auto get_focused() const -> focused_t override;
        [[nodiscard]] auto set_focused(Util::In<focused_t>) -> Util::Future<void> override;

        [[nodiscard]] auto get_position() const -> position_t override;
        [[nodiscard]] auto set_position(Util::In<position_t>) -> Util::Future<void> override;

        [[nodiscard]] auto get_dimension() const -> dimension_t override;
        [[nodiscard]] auto set_dimension(Util::In<dimension_t>) -> Util::Future<void> override;

        [[nodiscard]] auto get_title() const -> title_t override;
        [[nodiscard]] auto set_title(Util::In<title_t>) -> Util::Future<void> override;

        [[nodiscard]] auto get_swap_interval() const -> swap_interval_t override;
        [[nodiscard]] auto set_swap_interval(Util::In<swap_interval_t>) -> Util::Future<void> override;

    protected:
        void on_event(WindowEvent_Init const&) override;
        void on_event(WindowEvent_Dispose const&) override;
        void on_event(WindowEvent_Refresh const&) override;

    private:
        class Context; friend Context;
        std::unique_ptr<Context> m_Context;

        using Handle = void;
        Handle* m_Handle = nullptr;
    };

} // namespace RockHopper::UI::GLFW

#endif /* ROCKHOPPER_UI_WINDOW_GLFW_HH */

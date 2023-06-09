
#ifndef ROCKHOPPER_UI_WINDOW_GLFW_HH
#define ROCKHOPPER_UI_WINDOW_GLFW_HH

#include "RockHopper/UI/Window.hh"

#include "RockHopper/Util/Singleton.hh"

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
        [[nodiscard]] auto set_visible(Util::In<visible_t>) -> std::future<void> override;

        [[nodiscard]] auto get_focused() const -> focused_t override;
        [[nodiscard]] auto set_focused(Util::In<focused_t>) -> std::future<void> override;

        [[nodiscard]] auto get_dimensions() const -> dimension_t override;
        [[nodiscard]] auto set_dimensions(Util::In<dimension_t>) -> std::future<void> override;

        [[nodiscard]] auto get_title() const -> title_t override;
        [[nodiscard]] auto set_title(Util::In<title_t>) -> std::future<void> override;

        [[nodiscard]] auto get_swap_interval() const -> swap_interval_t override;
        [[nodiscard]] auto set_swap_interval(Util::In<swap_interval_t>) -> std::future<void> override;

    private:
        class Context; friend Context;
        Util::Singleton<Context> m_Context;

        struct Handle;
        Handle* m_Handle = nullptr;
    };

} // namespace RockHopper::UI::GLFW

#endif /* ROCKHOPPER_UI_WINDOW_GLFW_HH */

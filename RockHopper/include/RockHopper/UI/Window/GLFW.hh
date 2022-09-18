
#ifndef __HH_ROCKHOPPER_UI_WINDOW_GLFW_
#define __HH_ROCKHOPPER_UI_WINDOW_GLFW_

#include "RockHopper/UI/Window.hh"

namespace RockHopper::UI::GLFW
{

    class Window
        : public UI::Window
    {
    public:
        virtual ~Window();

        explicit Window(
            std::shared_ptr<Chrono::I_TickThread> const&,
            std::shared_ptr<GX::Renderer> const& );

        Window(Window&&);
        Window& operator=(Window&&);

    public:
        virtual void on_event(WindowEvent_Init const&) override;
        virtual void on_event(WindowEvent_Dispose const&) override;
        virtual void on_event(WindowEvent_Refresh const&) override;

    public:
        virtual bool is_visible() const override;
        virtual auto show() -> std::future<void> override;
        virtual auto hide() -> std::future<void> override;

        virtual auto get_width() const -> width_t override;
        virtual auto set_width(width_t const&) -> std::future<void> override;

        virtual auto get_height() const -> height_t override;
        virtual auto set_height(height_t const&) -> std::future<void> override;

        virtual auto get_title() const -> title_t override;
        virtual auto set_title(title_t const&) -> std::future<void> override;

        virtual auto get_swap_interval() const -> swap_interval_t override;
        virtual auto set_swap_interval(swap_interval_t const&) -> std::future<void> override;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_Impl;
    };

} // namespace RockHopper::UI::GLFW

#endif /* __HH_ROCKHOPPER_UI_WINDOW_GLFW_ */

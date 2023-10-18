
#include "RockHopper/UI/Window/GLFW.hh"
#include "RockHopper/UI/Window/GLFW.Context.hh"

#include "RockHopper/Logging/LoggerCore.enable.hh"

#include <GLFW/glfw3.h>

namespace RockHopper::UI::GLFW
{

    Window::~Window()
    {
        UI::Window::stop();
    }

    Window::Window()
        : m_Context{std::make_unique<Context>()}
    {
        UI::Window::start();
    }

    void Window::on_event(WindowEvent_Init const&)
    {
        m_Context->create_window(&m_Handle);
    }

    void Window::on_event(WindowEvent_Dispose const&)
    {
        m_Context->dispose_window(&m_Handle);
    }

    void Window::on_event(WindowEvent_Refresh const&)
    {
    }

    auto Window::get_visible() const -> visible_t
    {
        // TODO: Window::get_visible
    }

    auto Window::set_visible(Util::In<visible_t> visible) -> Util::Future<void>
    {
        // TODO: Window::set_visible
    }

    auto Window::get_focused() const -> focused_t
    {
        // TODO: Window::get_focused
    }

    auto Window::set_focused(Util::In<focused_t> focused) -> Util::Future<void>
    {
        // TODO: Window::set_focused
    }

    auto Window::get_position() const -> position_t
    {
        // TODO: Window::get_position
    }

    auto Window::set_position(Util::In<position_t> position) -> Util::Future<void>
    {
        // TODO: Window::set_position
    }

    auto Window::get_dimension() const -> dimension_t
    {
        // TODO: Window::get_dimension
    }

    auto Window::set_dimension(Util::In<dimension_t> dimension) -> Util::Future<void>
    {
        // TODO: Window::set_dimension
    }

    auto Window::get_title() const -> title_t
    {
        // TODO: Window::get_title
    }

    auto Window::set_title(Util::In<title_t> title) -> Util::Future<void>
    {
        // TODO: Window::set_title
    }

    auto Window::get_swap_interval() const -> swap_interval_t
    {
        // TODO: Window::get_swap_interval
    }

    auto Window::set_swap_interval(Util::In<swap_interval_t> swap_interval) -> Util::Future<void>
    {
        // TODO: Window::set_swap_interval
    }

} // namespace RockHopper::UI::GLFW


#include "RockHopper/UI/Window/GLFW.hh"
#include "RockHopper/UI/Window/GLFW.Context.hh"

namespace RockHopper::UI::GLFW
{

    Window::~Window()
    {
    }

    Window::Window()
    {
    }

    auto Window::get_visible() const -> visible_t
    {
        auto future = m_Context->get_property<visible_t>(m_Handle);
        future.wait();
        return future.get();
    }

    auto Window::set_visible(Util::In<visible_t> visible) -> std::future<void>
    {
        return m_Context->set_property<visible_t>(m_Handle,visible);
    }

    auto Window::get_focused() const -> focused_t
    {
        auto future = m_Context->get_property<focused_t>(m_Handle);
        future.wait();
        return future.get();
    }

    auto Window::set_focused(Util::In<focused_t> focused) -> std::future<void>
    {
        return m_Context->set_property<focused_t>(m_Handle,focused);
    }

    auto Window::get_dimensions() const -> dimension_t
    {
        auto future = m_Context->get_property<dimension_t>(m_Handle);
        future.wait();
        return future.get();
    }

    auto Window::set_dimensions(Util::In<dimension_t> dimensions) -> std::future<void>
    {
        return m_Context->set_property<dimension_t>(m_Handle,dimensions);
    }

    auto Window::get_title() const -> title_t
    {
        auto future = m_Context->get_property<title_t>(m_Handle);
        future.wait();
        return future.get();
    }

    auto Window::set_title(Util::In<title_t> title) -> std::future<void>
    {
        return m_Context->set_property<title_t>(m_Handle,title);
    }

    auto Window::get_swap_interval() const -> swap_interval_t
    {
        auto future = m_Context->get_property<swap_interval_t>(m_Handle);
        future.wait();
        return future.get();
    }

    auto Window::set_swap_interval(Util::In<swap_interval_t> swap_interval) -> std::future<void>
    {
        return m_Context->set_property<swap_interval_t>(m_Handle,swap_interval);
    }

} // namespace RockHopper::UI::GLFW

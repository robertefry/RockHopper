
#include "RockHopper/UI/Window/GLFW.hh"
#include "RockHopper/UI/Window/GLFW.Context.hh"

#include "RockHopper/Logging/LoggerCore.enable.hh"

#include <GLFW/glfw3.h>

namespace RockHopper::UI::GLFW
{

    Window::~Window()
    {
        m_Context->dispose_window(&m_Handle);
    }

    Window::Window()
        : m_Context{}
    {
        m_Context->create_window(&m_Handle);
    }

    auto Window::get_visible() const -> visible_t
    {
        return m_Context->get_property<visible_t>(m_Handle);
    }

    auto Window::set_visible(Util::In<visible_t> visible) -> Util::Future<void>
    {
        return m_Context->set_property<visible_t>(m_Handle,visible);
    }

    auto Window::get_focused() const -> focused_t
    {
        return m_Context->get_property<focused_t>(m_Handle);
    }

    auto Window::set_focused(Util::In<focused_t> focused) -> Util::Future<void>
    {
        return m_Context->set_property<focused_t>(m_Handle,focused);
    }

    auto Window::get_position() const -> position_t
    {
        return m_Context->get_property<position_t>(m_Handle);
    }

    auto Window::set_position(Util::In<position_t> position) -> Util::Future<void>
    {
        return m_Context->set_property<position_t>(m_Handle,position);
    }

    auto Window::get_dimension() const -> dimension_t
    {
        return m_Context->get_property<dimension_t>(m_Handle);
    }

    auto Window::set_dimension(Util::In<dimension_t> dimension) -> Util::Future<void>
    {
        return m_Context->set_property<dimension_t>(m_Handle,dimension);
    }

    auto Window::get_title() const -> title_t
    {
        return m_Context->get_property<title_t>(m_Handle);
    }

    auto Window::set_title(Util::In<title_t> title) -> Util::Future<void>
    {
        return m_Context->set_property<title_t>(m_Handle,title);
    }

    auto Window::get_swap_interval() const -> swap_interval_t
    {
        return m_Context->get_property<swap_interval_t>(m_Handle);
    }

    auto Window::set_swap_interval(Util::In<swap_interval_t> swap_interval) -> Util::Future<void>
    {
        return m_Context->set_property<swap_interval_t>(m_Handle,swap_interval);
    }

} // namespace RockHopper::UI::GLFW


#include "RockHopper/UI/Window/GLFW.Context.hh"

#include "RockHopper/Logging/LoggerCore.enable.hh"
#include "RockHopper/Logging/Asserts.hh"

#include <GLFW/glfw3.h>

namespace RockHopper::UI::GLFW
{

    static void GLFW_ErrorCallback(int error, const char* description)
    {
        ROCKHOPPER_LOG_ERROR("GLFW Error {}: {}", error, description);
    }

    Window::Context::~Context()
    {
        try
        {
            auto task = m_Renderer.push_task([]()
            {
                ROCKHOPPER_LOG_INFO("GLFW terminating");
                glfwTerminate();
            });
            task.wait();
        }
        catch(std::exception const& e)
        {
            ROCKHOPPER_LOG_FATAL("Exception occoured on GLFW Context destruction.\n{}",e.what());
        }
    }

    Window::Context::Context()
    {
        auto task = m_Renderer.push_task([]()
        {
            ROCKHOPPER_LOG_INFO("GLFW initializing");
            glfwSetErrorCallback(GLFW_ErrorCallback);

            int const status = glfwInit();
            ROCKHOPPER_ASSERT_FATAL(status,"GLFW initialization failed");

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,s_VersionMajor);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,s_VersionMinor);
        });
    }

    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define ExplicitlyInstantiateSetProperty(property_t) \
    template auto Window::Context::set_property<property_t> \
        (Window::Handle*,Util::In<property_t>) -> std::future<void>

    template <typename T_Property>
    auto Window::Context::set_property(
        Window::Handle* handle, Util::In<T_Property> property) -> std::future<void>
    {
        return m_Renderer.push_task([=,this,property=std::move(property)]()
        {
            PropertyCache& cache = m_PropertyMap[handle];
            std::unique_lock const lock {cache.m_Mutex};

            if constexpr (std::is_same_v<T_Property,Window::visible_t>)       { /* todo */ }
            if constexpr (std::is_same_v<T_Property,Window::focused_t>)       { /* todo */ }
            if constexpr (std::is_same_v<T_Property,Window::dimension_t>)     { /* todo */ }
            if constexpr (std::is_same_v<T_Property,Window::title_t>)         { /* todo */ }
            if constexpr (std::is_same_v<T_Property,Window::swap_interval_t>) { /* todo */ }

            std::get<T_Property>(cache.m_Tuple) = property;
        });
    }

    ExplicitlyInstantiateSetProperty(Window::visible_t);
    ExplicitlyInstantiateSetProperty(Window::focused_t);
    ExplicitlyInstantiateSetProperty(Window::dimension_t);
    ExplicitlyInstantiateSetProperty(Window::title_t);
    ExplicitlyInstantiateSetProperty(Window::swap_interval_t);

    // NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
    #define ExplicitlyInstantiateGetProperty(property_t) \
    template auto Window::Context::get_property<property_t> \
        (Window::Handle* handle) -> std::future<property_t>

    template <typename T_Property>
    auto Window::Context::get_property(
        Window::Handle* handle) -> std::future<T_Property>
    {
        return m_Renderer.push_task([=,this]() -> T_Property
        {
            PropertyCache& cache = m_PropertyMap[handle];
            std::shared_lock const lock {cache.m_Mutex};

            return std::get<T_Property>(cache.m_Tuple);
        });
    }

    ExplicitlyInstantiateGetProperty(Window::visible_t);
    ExplicitlyInstantiateGetProperty(Window::focused_t);
    ExplicitlyInstantiateGetProperty(Window::dimension_t);
    ExplicitlyInstantiateGetProperty(Window::title_t);
    ExplicitlyInstantiateGetProperty(Window::swap_interval_t);

} // namespace RockHopper::UI::GLFW

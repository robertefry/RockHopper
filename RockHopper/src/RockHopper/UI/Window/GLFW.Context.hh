
#ifndef ROCKHOPPER_UI_WINDOW_GLFW_CONTEXT_HH
#define ROCKHOPPER_UI_WINDOW_GLFW_CONTEXT_HH

#include "RockHopper/UI/Window/GLFW.hh"

#include "RockHopper/GX/Renderer.hh"

#include "RockHopper/Util/Lifetime.hh"

#include <tuple>
#include <unordered_map>
#include <shared_mutex>

namespace RockHopper::UI::GLFW
{

    static inline constexpr unsigned int s_VersionMajor = 3;
    static inline constexpr unsigned int s_VersionMinor = 3;

    class Window::Context final : private Util::NoMove
    {
    public:
        virtual ~Context();
        explicit Context();

        [[nodiscard]] auto renderer() & -> GX::Renderer& { return m_Renderer; };
        [[nodiscard]] auto renderer() const& -> GX::Renderer const& { return m_Renderer; };

    public:
        void create_window(Window::Handle**) noexcept;
        void dispose_window(Window::Handle**) noexcept;

        template <typename T_Property>
        auto set_property(Window::Handle*, Util::In<T_Property>) -> std::future<void>;
        template <typename T_Property>
        auto get_property(Window::Handle*) -> T_Property;

    private:
        GX::Renderer m_Renderer{};

        struct PropertyCache
        {
            using Tuple = std::tuple<
                visible_t,
                focused_t,
                position_t,
                dimension_t,
                title_t,
                swap_interval_t>;
            Tuple m_Tuple {};

            std::shared_mutex m_Mutex {};
        };
        using PropertyMap = std::unordered_map<Window::Handle*,PropertyCache>;
        PropertyMap m_PropertyMap{};
    };

} // namespace RockHopper::UI::GLFW

#endif /* ROCKHOPPER_UI_WINDOW_GLFW_CONTEXT_HH */


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

    public:
        template <typename T_Property>
        auto set_property(Window::Handle*, Util::In<T_Property>) -> std::future<void>;
        template <typename T_Property>
        auto get_property(Window::Handle*) const -> std::future<T_Property>;

    private:
        GX::Renderer mutable m_Renderer{};

        struct PropertyCache
        {
            using Tuple = std::tuple<visible_t,focused_t,dimension_t,title_t,swap_interval_t>;
            Tuple m_Tuple {};

            std::shared_mutex mutable m_Mutex {};
        };
        using PropertyMap = std::unordered_map<Window::Handle*,PropertyCache>;
        PropertyMap mutable m_PropertyMap{};
    };

} // namespace RockHopper::UI::GLFW

#endif /* ROCKHOPPER_UI_WINDOW_GLFW_CONTEXT_HH */

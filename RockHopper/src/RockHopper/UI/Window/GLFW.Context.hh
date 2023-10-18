
#ifndef ROCKHOPPER_UI_WINDOW_GLFW_CONTEXT_HH
#define ROCKHOPPER_UI_WINDOW_GLFW_CONTEXT_HH

#include "RockHopper/UI/Window/GLFW.hh"

#include "RockHopper/GX/Renderer.hh"

namespace RockHopper::UI::GLFW
{

    static inline constexpr unsigned int s_VersionMajor = 3;
    static inline constexpr unsigned int s_VersionMinor = 3;

    class Window::Context final
    {
    public:
        virtual ~Context();
        explicit Context();

        Context(Context const&) = default;
        Context& operator=(Context const&) = default;

        void create_window(Window::Handle**) noexcept;
        void dispose_window(Window::Handle**) noexcept;

    private:
        GX::Renderer m_Renderer{};
    };

} // namespace RockHopper::UI::GLFW

#endif /* ROCKHOPPER_UI_WINDOW_GLFW_CONTEXT_HH */

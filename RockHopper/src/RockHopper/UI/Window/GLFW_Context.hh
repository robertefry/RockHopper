
#ifndef __HH_ROCKHOPPER_UI_WINDOW_GLFW_CONTEXT_
#define __HH_ROCKHOPPER_UI_WINDOW_GLFW_CONTEXT_

#include "RockHopper/GX/Renderer.hh"

#include <cstddef>
#include <memory>

#define ROCKHOPPER_GLFW_VERSION_MAJOR 3
#define ROCKHOPPER_GLFW_VERSION_MINOR 0

namespace RockHopper::UI::GLFW
{

    class GLFW_Context
    {
    public:
        virtual ~GLFW_Context();

        explicit GLFW_Context(
            std::shared_ptr<GX::Renderer> const& renderer );

        GLFW_Context(GLFW_Context const&);
        GLFW_Context& operator=(GLFW_Context const&);

        GLFW_Context(GLFW_Context&&);
        GLFW_Context& operator=(GLFW_Context&&);

    public:
        struct Impl;
        static inline Impl* s_Impl = nullptr;
        static inline std::size_t s_UseCount = 0;
    };

} // namespace RockHopper::UI::GLFW

#endif /* __HH_ROCKHOPPER_UI_WINDOW_GLFW_CONTEXT_ */

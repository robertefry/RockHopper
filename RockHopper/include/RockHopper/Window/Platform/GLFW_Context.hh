
#ifndef __HH_ROCKHOPPER_WINDOW_GLFW_CONTEXT_
#define __HH_ROCKHOPPER_WINDOW_GLFW_CONTEXT_

#include "RockHopper/Rendering/Renderer.hh"

#include <atomic>

#define ROCKHOPPER_GLFW_VERSION_MAJOR 3
#define ROCKHOPPER_GLFW_VERSION_MINOR 0

class GLFWwindow;

namespace RockHopper
{

    class GlfwContext
    {
    public:
        virtual ~GlfwContext();
        explicit GlfwContext();

        explicit GlfwContext(GlfwContext const&);
        GlfwContext& operator=(GlfwContext const&);

        template <typename T_Device, bool T_Enable>
        std::future<void> set_callbacks(GLFWwindow* handle, T_Device* device);

    private:
        RenderThread m_RenderThread{};

        static inline std::atomic<unsigned> s_NumInstances = 0;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WINDOW_GLFW_CONTEXT_ */

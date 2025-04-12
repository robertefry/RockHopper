
#ifndef __HH_ROCKHOPPER_WINDOW_GLFW_CONTEXT_
#define __HH_ROCKHOPPER_WINDOW_GLFW_CONTEXT_

#include <future>
#include <atomic>

#define ROCKHOPPER_GLFW_VERSION_MAJOR 3
#define ROCKHOPPER_GLFW_VERSION_MINOR 0

class GLFWwindow;

namespace RockHopper
{

    class GLFW_Context
    {
    public:
        virtual ~GLFW_Context();
        explicit GLFW_Context();

        explicit GLFW_Context(GLFW_Context const&);
        GLFW_Context& operator=(GLFW_Context const&);

        template <typename T_Device, bool T_Enable>
        std::future<void> set_callbacks(GLFWwindow** handle, T_Device* device);

    private:
        static inline std::atomic<unsigned> s_NumInstances = 0;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WINDOW_GLFW_CONTEXT_ */

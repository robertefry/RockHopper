
#ifndef __HH_ROCKHOPPER_WINDOW_BACKEND_GLFW_CONTEXT_
#define __HH_ROCKHOPPER_WINDOW_BACKEND_GLFW_CONTEXT_

#include <atomic>

#define ROCKHOPPER_GLFW_VERSION_MAJOR 3
#define ROCKHOPPER_GLFW_VERSION_MINOR 0

class GLFWwindow;

namespace RockHopper
{

    class GLFW_Context
    {
    public:
        static void Register();
        static void Deregister();

        template <bool>
        static void SetWindowGLFWCallbacks(GLFWwindow* handle);

        template <bool>
        static void SetKeyboardGLFWCallbacks(GLFWwindow* handle);

        template <bool>
        static void SetMouseGLFWCallbacks(GLFWwindow* handle);

    private:
        inline static std::atomic<unsigned> m_Registered = 0;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WINDOW_BACKEND_GLFW_CONTEXT_ */

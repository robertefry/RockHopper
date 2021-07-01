
#ifndef __HH_ROCKHOPPER_WINDOW_BACKEND_GLFW_CONTEXT_
#define __HH_ROCKHOPPER_WINDOW_BACKEND_GLFW_CONTEXT_

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
        inline static unsigned m_Registered = 0;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WINDOW_BACKEND_GLFW_CONTEXT_ */

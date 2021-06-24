
#ifndef __HH_ROCKHOPPER_GLFW_CONTEXT_
#define __HH_ROCKHOPPER_GLFW_CONTEXT_

class GLFWwindow;

namespace RockHopper
{

    class GLFW_Context
    {
    public:
        static void Register();
        static void Deregister();
    private:
        inline static bool m_Registered = 0;
    };

    template <bool>
    void SetWindowGLFWCallbacks(GLFWwindow* handle);

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_GLFW_CONTEXT_ */

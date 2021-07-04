
#ifndef __HH_ROCKHOPPER_WINDOW_BACKEND_OPENGL_CONTEXT_
#define __HH_ROCKHOPPER_WINDOW_BACKEND_OPENGL_CONTEXT_

#include <atomic>

#define ROCKHOPPER_GLSL_VERSION "#version 130"

namespace RockHopper
{

    class OpenGL_Context
    {
    public:
        static void Register();
        static void Deregister();

    private:
        inline static std::atomic<unsigned> m_Registered = 0;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WINDOW_BACKEND_OPENGL_CONTEXT_ */

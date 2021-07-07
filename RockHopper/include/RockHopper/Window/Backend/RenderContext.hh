
#ifndef __HH_ROCKHOPPER_WINDOW_BACKEND_OPENGL_CONTEXT_
#define __HH_ROCKHOPPER_WINDOW_BACKEND_OPENGL_CONTEXT_

#include "RockHopper/Window/Backend/GraphicsThread.hh"

#include <atomic>

#define ROCKHOPPER_GLSL_VERSION "#version 130"

class GLFWwindow;

namespace RockHopper
{

    class RenderContext
    {
    public:
        virtual ~RenderContext();
        explicit RenderContext(GraphicsThread const& thread);

        std::future<void> initialize(GLFWwindow*);
        std::future<void> dispose(GLFWwindow*);

    private:
        GraphicsThread m_GraphicsThread;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WINDOW_BACKEND_OPENGL_CONTEXT_ */

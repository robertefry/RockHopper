
#ifndef __HH_ROCKHOPPER_WINDOW_BACKEND_OPENGL_CONTEXT_
#define __HH_ROCKHOPPER_WINDOW_BACKEND_OPENGL_CONTEXT_

#define ROCKHOPPER_GLSL_VERSION "#version 130"

namespace RockHopper
{

    class RenderContext
    {
    public:
        void initialize();
        void dispose();

        void refresh();
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WINDOW_BACKEND_OPENGL_CONTEXT_ */

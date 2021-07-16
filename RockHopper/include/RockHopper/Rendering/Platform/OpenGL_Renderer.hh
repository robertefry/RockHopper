
#ifndef __HH_ROCKHOPPER_RENDERER_OPENGL_
#define __HH_ROCKHOPPER_RENDERER_OPENGL_

#include "RockHopper/Rendering/Renderer.hh"

namespace RockHopper
{

    class OpenGL_Renderer : public Renderer
    {
    public:
        virtual ~OpenGL_Renderer() = default;

        virtual void initialize() override;
        virtual void dispose() override;
        virtual void refresh() override;

        virtual void submit(Shader const&, Mesh const&) override;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_RENDERER_OPENGL_ */

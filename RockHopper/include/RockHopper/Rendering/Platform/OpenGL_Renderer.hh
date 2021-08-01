
#ifndef __HH_ROCKHOPPER_RENDERER_OPENGL_
#define __HH_ROCKHOPPER_RENDERER_OPENGL_

#include "RockHopper/Rendering/Renderer.hh"

namespace RockHopper
{

    class OpenGL_Renderer : public Renderer
    {
    public:
        virtual ~OpenGL_Renderer();
        explicit OpenGL_Renderer();

        virtual void scene_begin() override;
        virtual void scene_end() override;

        virtual void refresh() override;
        virtual void submit(Shader const&, Mesh const&) override;

    private:
        virtual void init() override;
        virtual void dispose() override;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_RENDERER_OPENGL_ */

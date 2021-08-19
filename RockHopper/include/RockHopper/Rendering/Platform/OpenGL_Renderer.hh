
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

        explicit OpenGL_Renderer(OpenGL_Renderer const&) = delete;
        OpenGL_Renderer& operator=(OpenGL_Renderer const&) = delete;

        virtual void viewport(float x, float y, float w, float h) override;

        virtual void scene_begin(Camera*) override;
        virtual void scene_end() override;

        virtual void submit(Shader&, Mesh&) override;

    private:
        virtual void init() override;
        virtual void dispose() override;

    private:
        Camera* m_SceneCamera{};
        bool m_SceneInProgress = false;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_RENDERER_OPENGL_ */

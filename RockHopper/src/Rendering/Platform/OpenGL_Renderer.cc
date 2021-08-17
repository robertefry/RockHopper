
#include "RockHopper/Rendering/Platform/OpenGL_Renderer.hh"

#include "RockHopper/Debug.hh"
#include "RockHopper/Rendering/Shader.hh"
#include "RockHopper/Rendering/Mesh.hh"

#include <glad/glad.h>

namespace RockHopper
{

    OpenGL_Renderer::~OpenGL_Renderer()
    {
    }

    OpenGL_Renderer::OpenGL_Renderer()
        : Renderer{"OpenGL"}
    {
    }

    void OpenGL_Renderer::viewport(float x, float y, float w, float h)
    {
        push_task([=]()
        {
            glViewport(x,y,w,h);
        });
    }

    void OpenGL_Renderer::init()
    {
        push_task([this]()
        {
            ROCKHOPPER_INTERNAL_LOG_INFO("Loading GLAD OpenGL.");
            int status = gladLoadGL();
            ROCKHOPPER_INTERNAL_ASSERT_FATAL(status,"GLAD failed to load OpenGL!");

            const GLubyte* version = glGetString(GL_VERSION);
            ROCKHOPPER_INTERNAL_LOG_INFO("Initialized GLAD OpenGL {}.",version);

            // Enable counter-clockwise face culling
            glFrontFace(GL_CCW);
            glCullFace(GL_BACK);
            glEnable(GL_CULL_FACE);

            // Enable SRGB colours
            glEnable(GL_FRAMEBUFFER_SRGB);
        });
    }

    void OpenGL_Renderer::dispose()
    {
    }

    void OpenGL_Renderer::scene_begin()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGL_Renderer::scene_end()
    {
    }

    void OpenGL_Renderer::submit(Shader const& shader, Mesh const& mesh)
    {
        shader.bind();
        mesh.bind();
        glDrawElements(GL_TRIANGLES,mesh.num_indices(),GL_UNSIGNED_INT,nullptr);
    }

} // namespace RockHopper

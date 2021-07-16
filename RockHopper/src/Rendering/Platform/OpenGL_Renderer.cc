
#include "RockHopper/Rendering/Platform/OpenGL_Renderer.hh"
#include "RockHopper/Rendering/Shader.hh"
#include "RockHopper/Rendering/Mesh.hh"

#include "RockHopper/Debug.hh"

#include <glad/glad.h>

namespace RockHopper
{

    void OpenGL_Renderer::initialize()
    {
        ROCKHOPPER_INTERNAL_LOG_INFO("Loading GLAD OpenGL.");
        int status = gladLoadGL();
        ROCKHOPPER_INTERNAL_ASSERT_FATAL(status,"GLAD failed to load OpenGL!");

        const GLubyte* version = glGetString(GL_VERSION);
        ROCKHOPPER_INTERNAL_LOG_INFO("Initialized GLAD OpenGL {}.",version);
    }

    void OpenGL_Renderer::dispose()
    {
    }

    void OpenGL_Renderer::refresh()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGL_Renderer::submit(Shader const& shader, Mesh const& mesh)
    {
        shader.bind();
        mesh.bind();
        glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,nullptr);
    }

} // namespace RockHopper

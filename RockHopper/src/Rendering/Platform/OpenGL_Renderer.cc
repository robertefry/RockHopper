
#include "RockHopper/Rendering/Platform/OpenGL_Renderer.hh"

#include "RockHopper/Debug.hh"
#include "RockHopper/Rendering/Camera/Camera.hh"
#include "RockHopper/Rendering/Shader.hh"
#include "RockHopper/Rendering/Mesh.hh"
#include "RockHopper/Rendering/Transform.hh"

#include <glad/glad.h>

#include <functional>

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

    void OpenGL_Renderer::scene_begin(Camera* camera)
    {
        ROCKHOPPER_INTERNAL_ASSERT_FATAL((m_SceneInProgress == false),"Only one scene allowed per renderer!");
        m_SceneInProgress = true;
        m_SceneCamera = camera;

        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGL_Renderer::scene_end()
    {
        ROCKHOPPER_INTERNAL_ASSERT_FATAL((m_SceneInProgress == true),"Must begin the scene before ending the scene!");
        m_SceneInProgress = false;
#ifndef NDEBUG
        m_SceneCamera = nullptr;
#endif
    }

    void OpenGL_Renderer::submit(Shader& shader, Mesh& mesh, Transform& trans)
    {
        std::function const GetUniformData = [&](Shader::Uniform uniform) -> float*
        {
            switch (uniform)
            {
                case Shader::Uniform::CAMERA: {
                    return m_SceneCamera->data();
                } break;
                case Shader::Uniform::TRANSFORM: {
                    return trans.data();
                } break;
            }
            ROCKHOPPER_INTERNAL_LOG_FATAL("Unknown shader uniform type!");
            return nullptr;
        };

        shader.bind();
        mesh.bind();

        for (auto const& [uniform,name] : shader.uniform_map())
        {
            shader.set_uniform(name,GetUniformData(uniform));
        }

        glDrawElements(GL_TRIANGLES,mesh.num_indices(),GL_UNSIGNED_INT,nullptr);
    }

} // namespace RockHopper

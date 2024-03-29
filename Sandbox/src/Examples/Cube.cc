
#include "Cube.hh"

#include "RockHopper/Window/Window.hh"
#include "RockHopper/Rendering/Renderer.hh"
#include "RockHopper/Rendering/Camera/Camera.hh"

#include <glad/glad.h>

Cube::Cube()
{
}

void Cube::on_event(WindowInitEvent const& event)
{
    m_Shader = Shader::Create();
    {
        m_Shader->source_shader(Shader::Type::VERTEX,R"glsl(
            #version 330 core

            layout(location=0) in vec3 a_Position;
            layout(location=1) in vec4 a_Color;

            uniform mat4 u_Transform;
            uniform mat4 u_View;

            out vec3 v_Position;
            out vec4 v_Color;

            void main()
            {
                gl_Position = u_View * u_Transform * vec4(a_Position,1.0);
                v_Position = a_Position;
                v_Color = a_Color;
            }
        )glsl");
        m_Shader->source_shader(Shader::Type::FRAGMENT,R"glsl(
            #version 330 core

            in vec3 v_Position;
            in vec4 v_Color;

            layout(location=0) out vec4 o_Color;

            void main()
            {
                o_Color = v_Color;
            }
        )glsl");
        m_Shader->make_program();
        m_Shader->map_uniform(Shader::Uniform::CAMERA,"u_View");
        m_Shader->map_uniform(Shader::Uniform::TRANSFORM,"u_Transform");
    }

    m_Mesh = Mesh::Create();
    {
        Mesh::Data mesh_data;
        mesh_data.layout =
        {
            { Mesh::Data::Type::FLOAT, 3, false, "a_Position" },
            { Mesh::Data::Type::FLOAT, 4, false, "a_Color" },
        };
        mesh_data.vertices =
        {
            +0.0f, +0.0f, +0.0f, +0.0f, +0.0f, +0.0f, +0.0f,
            +1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +0.0f, +1.0f,
            +1.0f, -1.0f, -1.0f, +1.0f, +0.0f, +0.0f, +1.0f,
            +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f,
            +1.0f, -1.0f, +1.0f, +1.0f, +0.0f, +1.0f, +1.0f,
            -1.0f, +1.0f, -1.0f, +0.0f, +1.0f, +0.0f, +1.0f,
            -1.0f, -1.0f, -1.0f, +0.0f, +0.0f, +0.0f, +1.0f,
            -1.0f, +1.0f, +1.0f, +0.0f, +1.0f, +1.0f, +1.0f,
            -1.0f, -1.0f, +1.0f, +0.0f, +0.0f, +1.0f, +1.0f,
        };
        mesh_data.indices =
        {
            5, 3, 1,
            3, 8, 4,
            7, 6, 8,
            2, 8, 6,
            1, 4, 2,
            5, 2, 6,
            5, 7, 3,
            3, 7, 8,
            7, 5, 6,
            2, 4, 8,
            1, 3, 4,
            5, 1, 2,
        };
        m_Mesh->upload(mesh_data);
    }

    m_Transform = std::make_unique<Transform>();
    m_Transform->scale(0.75f);
}

void Cube::on_event(WindowDisposeEvent const& event)
{
    m_Shader.reset();
    m_Mesh.reset();
}

void Cube::on_event(EngineTickEvent const& event)
{
    m_SigmaTime += event.delta;
    m_Transform->rotate(event.delta.count()*0.7f,glm::vec3{0.5f,0.5f,0.2f});
    m_Transform->translate(0.005f*glm::vec3{cos(m_SigmaTime.count()*8),sin(m_SigmaTime.count()*8),0.0f});
}

void Cube::on_event(WindowRefreshEvent const& event)
{
    Renderer::GetInstance()->submit(*m_Shader,*m_Mesh,*m_Transform);
}

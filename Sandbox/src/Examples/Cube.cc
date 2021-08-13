
#include "Cube.hh"

#include "RockHopper/Rendering/Renderer.hh"

#include <glad/glad.h>

void Cube::on_event(WindowInitEvent const& event)
{
    m_Shader = Shader::Create();
    {
        m_Shader->source_shader(Shader::Type::VERTEX,R"glsl(
            #version 330 core

            layout(location=0) in vec3 a_Position;

            uniform float u_Scale;
            uniform mat4 u_View;

            out vec3 v_Position;
            out vec4 v_Colour;

            void main()
            {
                gl_Position = u_View * vec4(a_Position*u_Scale,1.0);
                v_Position = a_Position;
                v_Colour = vec4(a_Position*0.5+0.5,1.0);
            }
        )glsl");
        m_Shader->source_shader(Shader::Type::FRAGMENT,R"glsl(
            #version 330 core

            in vec3 v_Position;
            in vec4 v_Colour;

            layout(location=0) out vec4 o_Colour;

            void main()
            {
                o_Colour = v_Colour;
            }
        )glsl");
        m_Shader->make_program();
        m_Shader->bind();
        m_Shader->def_uniform("u_Scale",Shader::UniformType::SCALAR,1,0.5f);
        m_Shader->def_uniform("u_View",Shader::UniformType::MAT44,1,m_Camera.data());
    }
    m_Mesh = Mesh::Create();
    {
        Mesh::Data mesh_data;
        mesh_data.layout =
        {
            { Mesh::Data::Type::FLOAT, 3, false, "a_Position" },
        };
        mesh_data.vertices =
        {
            +0.0f, +0.0f, +0.0f,
            +1.0f, +1.0f, -1.0f,
            +1.0f, -1.0f, -1.0f,
            +1.0f, +1.0f, +1.0f,
            +1.0f, -1.0f, +1.0f,
            -1.0f, +1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, +1.0f, +1.0f,
            -1.0f, -1.0f, +1.0f,
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
}

void Cube::on_event(WindowDisposeEvent const& event)
{
    m_Shader.reset();
    m_Mesh.reset();
}

void Cube::on_event(EngineTickEvent const& event)
{
    m_SigmaTime = fmod(m_SigmaTime+(float)event.delta,2*M_PI);
    m_Camera.position(glm::vec3{0.5f*sin(m_SigmaTime),0.5f*cos(m_SigmaTime),0.0f});
    m_Camera.rotation(glm::vec3{0.5f*sin(m_SigmaTime),0.5f*cos(m_SigmaTime),0.0f});
}

void Cube::on_event(WindowRefreshEvent const& event)
{
    m_Shader->bind();
    m_Shader->set_uniform("u_View",m_Camera.data());

    Renderer::GetInstance()->submit(*m_Shader,*m_Mesh);
}


#include "Triangle.hh"

#include "RockHopper/Rendering/Renderer.hh"

#include <cmath>

Triangle::~Triangle()
{
}

Triangle::Triangle()
{
}

void Triangle::on_event(WindowInitEvent const& event)
{
    m_Shader = Shader::Create();
    {
        m_Shader->source_shader(Shader::Type::VERTEX,R"glsl(
            #version 330 core

            layout(location=0) in vec3 a_Position;
            layout(location=1) in vec4 a_Colour;

            uniform mat4 u_View;
            uniform float u_Scale;

            out vec3 v_Position;
            out vec4 v_Colour;

            void main()
            {
                gl_Position = u_View * vec4(a_Position*u_Scale,1.0);
                v_Position = a_Position;
                v_Colour = a_Colour;
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
        m_Shader->def_uniform("u_View",Shader::UniformType::MAT44,1,m_Camera.data());
        m_Shader->def_uniform("u_Scale",Shader::UniformType::SCALAR,1,m_Scale);
    }
    m_Mesh = Mesh::Create();
    {
        Mesh::Data mesh_data;
        mesh_data.layout =
        {
            { Mesh::Data::Type::FLOAT, 3, false, "a_Position" },
            { Mesh::Data::Type::FLOAT, 4, false, "a_Colour" },
        };
        mesh_data.vertices =
        {
            -0.5f, -0.5f, +0.0f, +1.0f, +0.0f, +0.0f, +1.0f,
            +0.5f, -0.5f, +0.0f, +0.0f, +1.0f, +0.0f, +1.0f,
            +0.0f, +0.5f, +0.0f, +0.0f, +0.0f, +1.0f, +1.0f,
        };
        mesh_data.indices =
        {
            0, 1, 2,
        };
        m_Mesh->upload(mesh_data);
    }
}

void Triangle::on_event(WindowDisposeEvent const& event)
{
    m_Shader.reset();
    m_Mesh.reset();
}

void Triangle::on_event(EngineTickEvent const& event)
{
    // m_Camera.rotate_z(event.delta*M_PI);
    m_Sigma = fmod(m_Sigma+(float)event.delta,2*M_PI);
    m_Camera.position(glm::vec3{sin(m_Sigma),0,0});
}

void Triangle::on_event(WindowRefreshEvent const& event)
{
    m_Shader->bind();
    m_Shader->set_uniform("u_View",m_Camera.data());

    Renderer::GetInstance()->submit(*m_Shader,*m_Mesh);
}

void Triangle::on_event(KeyPressEvent const& event)
{
    if (event.key == KeyCode::KEY_UP)
    {
        m_Scale += 0.1;

        Renderer::GetInstance()->push_task([this]()
        {
            m_Shader->bind();
            m_Shader->set_uniform("u_Scale",m_Scale);
        });
    }
    if (event.key == KeyCode::KEY_DOWN)
    {
        m_Scale -= 0.1;

        Renderer::GetInstance()->push_task([this]()
        {
            m_Shader->bind();
            m_Shader->set_uniform("u_Scale",m_Scale);
        });
    }
}

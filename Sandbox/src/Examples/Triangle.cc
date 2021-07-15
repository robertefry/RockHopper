
#include "Triangle.hh"

#include <glad/glad.h>

Triangle::~Triangle()
{
}

Triangle::Triangle()
{
}

void Triangle::on_event(WindowInitEvent const& event)
{
    // Compile the shader program
    m_Shader = Shader::Create();
    m_Shader->source_shader(Shader::Type::VERTEX,R"glsl(
        #version 330 core

        layout(location=0) in vec3 a_Position;
        layout(location=1) in vec4 a_Colour;
        out vec3 v_Position;
        out vec4 v_Colour;

        void main()
        {
            gl_Position = vec4(a_Position,1.0);
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

    // Create an OpenGL vertex array
    glGenVertexArrays(1,&m_VertexArray);
    glBindVertexArray(m_VertexArray);

    // Upload the vertex buffer data
    VertexBuffer::Data vertex_data;
    vertex_data.layout =
    {
        { VertexBuffer::FLOAT, 3, false, "a_Position" },
        { VertexBuffer::FLOAT, 4, false, "a_Colour" },
    };
    vertex_data.vertices =
    {
        -0.5f, -0.5f, +0.0f, +0.8f, +0.1f, +0.1f, +1.0f,
        +0.5f, -0.5f, +0.0f, +0.1f, +0.8f, +0.1f, +1.0f,
        +0.0f, +0.5f, +0.0f, +0.1f, +0.1f, +0.8f, +1.0f,
    };
    m_VertexBuffer = VertexBuffer::Create();
    m_VertexBuffer->upload(vertex_data);

    // Upload the index buffer data
    IndexBuffer::Data index_data;
    index_data.indices =
    {
        0, 1, 2,
    };
    m_IndexBuffer = IndexBuffer::Create();
    m_IndexBuffer->upload(index_data);
}

void Triangle::on_event(WindowDisposeEvent const& event)
{
    glDeleteVertexArrays(1,&m_VertexArray);
    m_VertexBuffer.reset();
    m_IndexBuffer.reset();
    m_Shader.reset();
}

void Triangle::on_event(WindowRefreshEvent const& event)
{
    m_Shader->bind();
    glBindVertexArray(m_VertexArray);
    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,nullptr);
}


#include "Triangle.hh"

#include <glad/glad.h>

Triangle::~Triangle()
{
}

Triangle::Triangle()
{
    m_Shader.source_shader(Shader::Type::VERTEX,R"glsl(
        #version 330 core

        layout(location=0) in vec3 a_Position;
        out vec3 v_Position;

        void main()
        {
            gl_Position = vec4(a_Position,1.0);
            v_Position = a_Position;
        }
    )glsl");
    m_Shader.source_shader(Shader::Type::FRAGMENT,R"glsl(
        #version 330 core

        layout(location=0) out vec4 o_Colour;
        in vec3 v_Position;

        void main()
        {
            o_Colour = vec4(v_Position*0.5+0.5,1.0);
        }
    )glsl");
    m_Shader.make_program();
}

void Triangle::on_event(WindowInitEvent const& event)
{
    // Create an OpenGL vertex array
    glGenVertexArrays(1,&m_VertexArray);
    glBindVertexArray(m_VertexArray);

    // Create an OpenGL vertex buffer
    glGenBuffers(1,&m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,m_VertexBuffer);

    // Fill the vertex buffer with data
    float vertices[] =
    {
        -0.5f,-0.5f,+0.0f,
        +0.5f,-0.5f,+0.0f,
        +0.0f,+0.5f,+0.0f,
    };
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),nullptr);

    // Create an OpenGL index buffer
    glGenBuffers(1,&m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_IndexBuffer);

    // Fill the index buffer with data
    unsigned int indices[] =
    {
        0, 1, 2,
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
}

void Triangle::on_event(WindowDisposeEvent const& event)
{
}

void Triangle::on_event(WindowRefreshEvent const& event)
{
    m_Shader.bind();
    glBindVertexArray(m_VertexBuffer);
    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,nullptr);
}

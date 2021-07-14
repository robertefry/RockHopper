
#include "RockHopper/Rendering/Platform/OpenGL_Buffer.hh"

#include <glad/glad.h>

/* ************************************************************************** */
// [Implementation] RockHopper::OpenGL_VertexBuffer
/* ************************************************************************** */

namespace RockHopper
{

    OpenGL_VertexBuffer::~OpenGL_VertexBuffer()
    {
        m_RenderingThread.push_task([buffer=m_VertexBuffer]()
        {
            if (buffer) glDeleteBuffers(1,&buffer);
        });
    }

    OpenGL_VertexBuffer::OpenGL_VertexBuffer()
    {
    }

    OpenGL_VertexBuffer::OpenGL_VertexBuffer(OpenGL_VertexBuffer&& other)
        : m_VertexBuffer{other.m_VertexBuffer}
    {
        other.m_VertexBuffer = 0;
    }

    OpenGL_VertexBuffer& OpenGL_VertexBuffer::operator=(OpenGL_VertexBuffer&& other)
    {
        std::swap(m_VertexBuffer,other.m_VertexBuffer);
        return *this;
    }

    void OpenGL_VertexBuffer::upload(std::vector<float> const& data)
    {
        if (not m_VertexBuffer) glGenBuffers(1,&m_VertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER,m_VertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,sizeof(float)*data.size(),data.data(),GL_STATIC_DRAW);
    }

    void OpenGL_VertexBuffer::bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER,m_VertexBuffer);
    }

    void OpenGL_VertexBuffer::unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }

} // namespace RockHopper

/* ************************************************************************** */
// [Implementation] RockHopper::OpenGL_IndexBuffer
/* ************************************************************************** */

namespace RockHopper
{

    OpenGL_IndexBuffer::~OpenGL_IndexBuffer()
    {
        m_RenderingThread.push_task([buffer=m_IndexBuffer]()
        {
            if (buffer) glDeleteBuffers(1,&buffer);
        });
    }

    OpenGL_IndexBuffer::OpenGL_IndexBuffer()
    {
    }

    OpenGL_IndexBuffer::OpenGL_IndexBuffer(OpenGL_IndexBuffer&& other)
        : m_IndexBuffer{other.m_IndexBuffer}
    {
        other.m_IndexBuffer = 0;
    }

    OpenGL_IndexBuffer& OpenGL_IndexBuffer::operator=(OpenGL_IndexBuffer&& other)
    {
        std::swap(m_IndexBuffer,other.m_IndexBuffer);
        return *this;
    }

    void OpenGL_IndexBuffer::upload(std::vector<uint32_t> const& data)
    {
        if (not m_IndexBuffer) glGenBuffers(1,&m_IndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_IndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(uint32_t)*data.size(),data.data(),GL_STATIC_DRAW);
    }

    void OpenGL_IndexBuffer::bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_IndexBuffer);
    }

    void OpenGL_IndexBuffer::unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    }

} // namespace RockHopper

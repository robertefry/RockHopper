
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
    {
        *this = std::move(other);
    }

    OpenGL_VertexBuffer& OpenGL_VertexBuffer::operator=(OpenGL_VertexBuffer&& other)
    {
        std::swap(m_VertexBuffer,other.m_VertexBuffer);
        return *this;
    }

    void OpenGL_VertexBuffer::upload(Data const& data)
    {
        if (not m_VertexBuffer) glGenBuffers(1,&m_VertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER,m_VertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,sizeof(float)*data.vertices.size(),data.vertices.data(),GL_STATIC_DRAW);

        std::function const GetTypeSize = [&](Type type) -> uint32_t
        {
            switch (type)
            {
                case Type::BOOL:  return 1;
                case Type::INT:   return 4;
                case Type::FLOAT: return 4;
            }
            ROCKHOPPER_INTERNAL_LOG_FATAL("Unknown vertex data type!");
            return 0;
        };
        std::function const GetTypeEnum = [&](Type type) -> GLenum
        {
            switch (type)
            {
                case Type::BOOL:  return GL_BOOL;
                case Type::INT:   return GL_INT;
                case Type::FLOAT: return GL_FLOAT;
            }
            ROCKHOPPER_INTERNAL_LOG_FATAL("Unknown vertex data type!");
            return 0;
        };

        uint32_t total_stride = 0;
        std::unordered_map<size_t,void*> pointer_map;
        for (size_t i = 0; i < data.layout.size(); ++i)
        {
            pointer_map[i] = reinterpret_cast<void*>(total_stride);
            total_stride += GetTypeSize(data.layout[i].type) * data.layout[i].count;
        }
        for (size_t i = 0; i < data.layout.size(); ++i)
        {
            auto const& elem = data.layout[i];
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i,elem.count,GetTypeEnum(elem.type),elem.normalized,total_stride,pointer_map[i]);
        }
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
    {
        *this = std::move(other);
    }

    OpenGL_IndexBuffer& OpenGL_IndexBuffer::operator=(OpenGL_IndexBuffer&& other)
    {
        std::swap(m_IndexBuffer,other.m_IndexBuffer);
        return *this;
    }

    void OpenGL_IndexBuffer::upload(Data const& data)
    {
        if (not m_IndexBuffer) glGenBuffers(1,&m_IndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_IndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(uint32_t)*data.indices.size(),data.indices.data(),GL_STATIC_DRAW);
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

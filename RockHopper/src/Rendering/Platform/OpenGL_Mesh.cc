
#include "RockHopper/Rendering/Platform/OpenGL_Mesh.hh"

#include "RockHopper/Debug.hh"

#include <glad/glad.h>
#include <functional>
#include <unordered_map>

namespace RockHopper
{

    OpenGL_Mesh::~OpenGL_Mesh()
    {
        glDeleteVertexArrays(1,&m_VertexArray);
        glDeleteBuffers(1,&m_VertexBuffer);
        glDeleteBuffers(1,&m_IndexBuffer);
    }

    OpenGL_Mesh::OpenGL_Mesh()
    {
        glGenVertexArrays(1,&m_VertexArray);
        glGenBuffers(1,&m_VertexBuffer);
        glGenBuffers(1,&m_IndexBuffer);
    }

    OpenGL_Mesh::OpenGL_Mesh(OpenGL_Mesh&& other)
    {
        *this = std::move(other);
    }

    OpenGL_Mesh& OpenGL_Mesh::operator=(OpenGL_Mesh&& other)
    {
        std::swap(m_VertexArray,other.m_VertexArray);
        std::swap(m_VertexBuffer,other.m_VertexBuffer);
        std::swap(m_IndexBuffer,other.m_IndexBuffer);
        return *this;
    }

    void OpenGL_Mesh::bind() const
    {
        glBindVertexArray(m_VertexArray);
    }

    void OpenGL_Mesh::unbind() const
    {
        glBindVertexArray(0);
    }

    size_t OpenGL_Mesh::num_vertices() const
    {
        return m_NumVertices;
    }

    size_t OpenGL_Mesh::num_indices() const
    {
        return m_NumIndices;
    }

    void OpenGL_Mesh::upload(Data const& data)
    {
        glBindVertexArray(m_VertexArray);

        glBindBuffer(GL_ARRAY_BUFFER,m_VertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,sizeof(float)*data.vertices.size(),data.vertices.data(),GL_STATIC_DRAW);
        m_NumVertices = data.vertices.size();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_IndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(uint32_t)*data.indices.size(),data.indices.data(),GL_STATIC_DRAW);
        m_NumIndices = data.indices.size();

        std::function const GetTypeSize = [&](Data::Type type) -> uint32_t
        {
            switch (type)
            {
                case Data::Type::BOOL:  return 1;
                case Data::Type::INT:   return 4;
                case Data::Type::FLOAT: return 4;
            }
            ROCKHOPPER_INTERNAL_LOG_FATAL("unsupported vertex data size");
            return 0;
        };
        std::function const GetTypeEnum = [&](Data::Type type) -> GLenum
        {
            switch (type)
            {
                case Data::Type::BOOL:  return GL_BOOL;
                case Data::Type::INT:   return GL_INT;
                case Data::Type::FLOAT: return GL_FLOAT;
            }
            ROCKHOPPER_INTERNAL_LOG_FATAL("unsupported vertex data type");
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

} // namespace RockHopper

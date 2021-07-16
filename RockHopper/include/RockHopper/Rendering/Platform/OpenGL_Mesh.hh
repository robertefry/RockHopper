
#ifndef __HH_ROCKHOPPER_RENDERING_OPENGL_MESH_
#define __HH_ROCKHOPPER_RENDERING_OPENGL_MESH_

#include "RockHopper/Rendering/Mesh.hh"

#include <cstdint>

namespace RockHopper
{

    class OpenGL_Mesh : public Mesh
    {
    public:
        virtual ~OpenGL_Mesh();
        explicit OpenGL_Mesh();

        explicit OpenGL_Mesh(OpenGL_Mesh&&);
        OpenGL_Mesh& operator=(OpenGL_Mesh&&);

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual void upload(Data const& data) override;

    private:
        uint32_t m_VertexArray{};
        uint32_t m_VertexBuffer{};
        uint32_t m_IndexBuffer{};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_RENDERING_OPENGL_MESH_ */

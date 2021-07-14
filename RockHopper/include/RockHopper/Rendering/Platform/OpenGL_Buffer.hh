
#ifndef __HH_ROCKHOPPER_RENDERING_OPENGL_BUFFER_
#define __HH_ROCKHOPPER_RENDERING_OPENGL_BUFFER_

#include "RockHopper/Rendering/Renderer.hh"
#include "RockHopper/Rendering/Buffer.hh"

namespace RockHopper
{

    class OpenGL_VertexBuffer : public VertexBuffer
    {
    public:
        virtual ~OpenGL_VertexBuffer();
        explicit OpenGL_VertexBuffer();

        explicit OpenGL_VertexBuffer(OpenGL_VertexBuffer&&);
        OpenGL_VertexBuffer& operator=(OpenGL_VertexBuffer&&);

        virtual void upload(Data const& data) override;

        virtual void bind() override;
        virtual void unbind() override;

    private:
        RenderThread m_RenderingThread{};
        uint32_t m_VertexBuffer{};
    };

    class OpenGL_IndexBuffer : public IndexBuffer
    {
    public:
        virtual ~OpenGL_IndexBuffer();
        explicit OpenGL_IndexBuffer();

        explicit OpenGL_IndexBuffer(OpenGL_IndexBuffer&&);
        OpenGL_IndexBuffer& operator=(OpenGL_IndexBuffer&&);

        virtual void upload(Data const& data) override;

        virtual void bind() override;
        virtual void unbind() override;

    private:
        RenderThread m_RenderingThread{};
        uint32_t m_IndexBuffer{};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_RENDERING_OPENGL_BUFFER_ */

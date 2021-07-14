
#include "RockHopper/Rendering/Buffer.hh"
#include "RockHopper/Rendering/Platform/OpenGL_Buffer.hh"

#include "RockHopper/Rendering/Renderer.hh"

namespace RockHopper
{

    std::unique_ptr<VertexBuffer> VertexBuffer::Create()
    {
        switch (Renderer::GetAPI())
        {
            case Renderer::API::OpenGL: return std::make_unique<OpenGL_VertexBuffer>();
            case Renderer::API::None: ROCKHOPPER_INTERNAL_LOG_FATAL("Cannot create a shader from the None API!");
        }
        ROCKHOPPER_INTERNAL_LOG_FATAL("Could not create a shader from an unknown API!");
        return nullptr;
    }

    std::unique_ptr<IndexBuffer> IndexBuffer::Create()
    {
        switch (Renderer::GetAPI())
        {
            case Renderer::API::OpenGL: return std::make_unique<OpenGL_IndexBuffer>();
            case Renderer::API::None: ROCKHOPPER_INTERNAL_LOG_FATAL("Cannot create a shader from the None API!");
        }
        ROCKHOPPER_INTERNAL_LOG_FATAL("Could not create a shader from an unknown API!");
        return nullptr;
    }

} // namespace RockHopper

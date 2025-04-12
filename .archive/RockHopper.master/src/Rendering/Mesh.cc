
#include "RockHopper/Rendering/Mesh.hh"
#include "RockHopper/Rendering/Platform/OpenGL_Mesh.hh"
#include "RockHopper/Rendering/Renderer.hh"

namespace RockHopper
{

    std::unique_ptr<Mesh> Mesh::Create()
    {
        switch (Renderer::GetAPI())
        {
            case Renderer::API::OpenGL: return std::make_unique<OpenGL_Mesh>();
            case Renderer::API::None: ROCKHOPPER_INTERNAL_LOG_FATAL("cannot create a mesh from the None API");
        }
        ROCKHOPPER_INTERNAL_LOG_FATAL("unsupported API");
        return nullptr;
    }

} // namespace RockHopper

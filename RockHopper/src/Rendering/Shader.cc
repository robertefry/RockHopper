
#include "RockHopper/Rendering/Shader.hh"
#include "RockHopper/Rendering/Platform/OpenGL_Shader.hh"

namespace RockHopper
{

    std::unique_ptr<Shader> Shader::Create()
    {
        switch (Renderer::GetAPI())
        {
            case Renderer::API::OpenGL: return std::make_unique<OpenGL_Shader>();
            case Renderer::API::None: ROCKHOPPER_INTERNAL_LOG_FATAL("Cannot create a shader from the None API!");
        }
        ROCKHOPPER_INTERNAL_LOG_FATAL("Could not create a shader from an unknown API!");
        return nullptr;
    }

} // namespace RockHopper

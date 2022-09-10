
#include "RockHopper/Rendering/Shader.hh"
#include "RockHopper/Rendering/Platform/OpenGL_Shader.hh"

#include "RockHopper/Rendering/Renderer.hh"

#include <functional>

namespace RockHopper
{

    std::unique_ptr<Shader> Shader::Create()
    {
        switch (Renderer::GetAPI())
        {
            case Renderer::API::OpenGL: return std::make_unique<OpenGL_Shader>();
            case Renderer::API::None: ROCKHOPPER_INTERNAL_LOG_FATAL("cannot create a shader from the None API");
        }
        ROCKHOPPER_INTERNAL_LOG_FATAL("unsupported API");
        return nullptr;
    }

    void Shader::map_uniform(Uniform uniform, std::string const& name)
    {
        std::function const GetUniformType = [](Uniform uniform)
        {
            switch (uniform)
            {
                case Uniform::CAMERA: return UniformType::MAT44;
                case Uniform::TRANSFORM: return UniformType::MAT44;
            }
            ROCKHOPPER_INTERNAL_LOG_FATAL("unsupported uniform type");
            return UniformType::VOID;
        };

        def_uniform(name,GetUniformType(uniform),1);
        m_UniformMap[uniform] = name;
    }

} // namespace RockHopper


#include "RockHopper/Rendering/Renderer.hh"
#include "RockHopper/Rendering/Platform/OpenGL_Renderer.hh"

#include "RockHopper/Debug.hh"
#include "RockHopper/Rendering/Shader.hh"
#include "RockHopper/Rendering/Mesh.hh"
#include "RockHopper/Rendering/Transform.hh"

namespace RockHopper
{

    Renderer::~Renderer()
    {
    }

    Renderer::Renderer(std::string const& api)
        : EngineThread{api}
    {
        m_DebugName.set_type("Renderer");
    }

    void Renderer::Create(API api)
    {
        if (s_Instance != nullptr)
        {
            ROCKHOPPER_INTERNAL_LOG_FATAL("cannot create multiple renderers");
        }
        s_RendererAPI = api;
        s_Instance = [&]() -> std::unique_ptr<Renderer>
        {
            switch (api)
            {
                case API::OpenGL: {
                    return std::make_unique<OpenGL_Renderer>();
                } break;
                case API::None: {
                    ROCKHOPPER_INTERNAL_LOG_FATAL("cannot create a renderer from the None API");
                } break;
            };
            ROCKHOPPER_INTERNAL_LOG_FATAL("unsupported API");
            return nullptr;
        }();
    }

    void Renderer::Destroy()
    {
        s_RendererAPI = API::None;
        s_Instance.reset();
    }

    void Renderer::submit(Shader& shader, Mesh& mesh)
    {
        Transform transform;
        submit(shader,mesh,transform);
    }

    void Renderer::tick()
    {
        execute_all();
    }

} // namespace RockHopper


#include "RockHopper/Rendering/Renderer.hh"
#include "RockHopper/Rendering/Platform/OpenGL_Renderer.hh"

#include "RockHopper/Debug.hh"

namespace RockHopper
{

    std::unique_ptr<Renderer> Renderer::Create(API api)
    {
        if (s_RenderAPI != API::None)
        {
            ROCKHOPPER_INTERNAL_LOG_FATAL("Cannot create multiple renderers!");
            return nullptr;
        }
        s_RenderAPI = api;

        switch (api)
        {
            case API::OpenGL: return std::make_unique<OpenGL_Renderer>();
            case API::None: ROCKHOPPER_INTERNAL_LOG_FATAL("Cannot create a renderer from the None API!");
        }
        ROCKHOPPER_INTERNAL_LOG_FATAL("Could not create a renderer from an unknown API!");
        return nullptr;
    }

} // namespace RockHopper

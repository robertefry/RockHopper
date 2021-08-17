
#include "RockHopper/Window/Window.hh"

namespace RockHopper
{

    Window::Window(Renderer::API renderer_api, std::string const& debug_name)
        : m_DebugName{"Window",debug_name}
        , m_Camera{std::make_unique<Camera>()}
    {
        // All window events must be dispatched on the graphics thread
        set_parallel_dispatch(false);

        // Create the renderer
        Renderer::Create(renderer_api);
    }

    Window::~Window()
    {
        // Destroy the renderer
        Renderer::Destroy();
    }

} // namespace RockHopper

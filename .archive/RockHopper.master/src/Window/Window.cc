
#include "RockHopper/Window/Window.hh"
#include "RockHopper/Event/EventListeners.hh"

namespace RockHopper
{

    Window::Window(Renderer::API renderer_api, std::string const& debug_name)
        : m_DebugName{"Window",debug_name}
        , m_Camera{std::make_unique<Camera>()}
    {
        Renderer::Create(renderer_api);

        insert_event_listener(m_Camera.get());
    }

    Window::~Window()
    {
        Renderer::Destroy();

        remove_event_listener(m_Camera.get());
    }

} // namespace RockHopper

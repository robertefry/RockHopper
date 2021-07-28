
#include "RockHopper/Window/Window.hh"

namespace RockHopper
{

    Window::Window(std::string const& debug_name)
        : m_DebugName{"Window",debug_name}
    {
        // All window events must be dispatched on the graphics thread
        set_parallel_dispatch(false);
    }

} // namespace RockHopper

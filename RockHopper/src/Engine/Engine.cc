
#include "RockHopper/Engine/Engine.hh"

namespace RockHopper
{

    Engine::Engine(std::string const& debug_name)
        : EngineThread{debug_name}
    {
    }

    void Engine::init()
    {
        EngineInitEvent event;
        event.engine = this;
        dispatch_event(event);
    }

    void Engine::tick()
    {
        EngineTickEvent event;
        event.engine = this;
        dispatch_event(event);
    }

    void Engine::dispose()
    {
        EngineDisposeEvent event;
        event.engine = this;
        dispatch_event(event);
    }

} // namespace RockHopper

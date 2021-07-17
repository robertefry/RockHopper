
#ifndef __HH_ROCKHOPPER_ENGINE_EVENT_
#define __HH_ROCKHOPPER_ENGINE_EVENT_

#include "RockHopper/Engine/Engine.fwd"
#include "RockHopper/Engine/EngineEvent.fwd"

#include "RockHopper/Event/Event.hh"

namespace RockHopper
{

    struct EngineEvent
        : EventCategory<EngineEventSet>
    {
        Engine* engine{};
    };

    struct EngineInitEvent : EventListenable<EngineInitEvent,EngineEvent>
    {
    };

    struct EngineDisposeEvent : EventListenable<EngineDisposeEvent,EngineEvent>
    {
    };

    struct EngineTickEvent : EventListenable<EngineTickEvent,EngineEvent>
    {
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_ENGINE_EVENT_ */

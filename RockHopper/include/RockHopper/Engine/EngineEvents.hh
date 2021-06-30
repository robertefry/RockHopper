
#ifndef __HH_ROCKHOPPER_ENGINE_EVENTS_
#define __HH_ROCKHOPPER_ENGINE_EVENTS_

#include "RockHopper/Engine/Engine.fwd"
#include "RockHopper/Engine/EngineEvents.fwd"

#include "RockHopper/Event/Event.hh"
#include "RockHopper/Event/EventHandler.hh"

namespace RockHopper
{

    struct EngineEvent
        : EventCategory<EngineEventSet>
    {
        Engine* engine{};
    };

    struct EngineInitEvent : EventListenable<EngineInitEvent,EngineEvent>
    {};

    struct EngineDisposeEvent : EventListenable<EngineDisposeEvent,EngineEvent>
    {};

    struct EngineTickEvent : EventListenable<EngineTickEvent,EngineEvent>
    {};

    struct EngineEventHandler
        : EventHandler<EngineEvent>
    {};

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_ENGINE_EVENTS_ */

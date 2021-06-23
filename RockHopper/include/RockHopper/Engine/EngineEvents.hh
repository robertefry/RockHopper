
#ifndef __HH_ROCKHOPPER_ENGINE_EVENTS_
#define __HH_ROCKHOPPER_ENGINE_EVENTS_

#include "RockHopper/Event/Event.hh"
#include "RockHopper/Event/EventHandler.hh"

namespace RockHopper
{

    struct EngineInitializationEvent;
    struct EngineTerminationEvent;

    using EngineEventSet = EventSet<EngineInitializationEvent,EngineTerminationEvent>;

    struct EngineEvent
        : I_EventListenable<EngineEventSet>
    {};

    struct EngineInitializationEvent : EventListenable<EngineInitializationEvent,EngineEvent>
    {};

    struct EngineTerminationEvent : EventListenable<EngineTerminationEvent,EngineEvent>
    {};

    struct EngineEventHandler
        : EventHandler<EngineEvent>
    {};

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_ENGINE_EVENTS_ */

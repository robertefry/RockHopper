
#ifndef __HH_ROCKHOPPER_ENGINE_EVENTS_
#define __HH_ROCKHOPPER_ENGINE_EVENTS_

#include "RockHopper/Event/EventHandler.hh"

namespace RockHopper
{

    struct EngineInitializationEvent
    {};

    struct EngineTerminationEvent
    {};

    class EngineEventHandler
        : public EventHandler<
            EngineInitializationEvent,
            EngineTerminationEvent >
    {};

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_ENGINE_EVENTS_ */

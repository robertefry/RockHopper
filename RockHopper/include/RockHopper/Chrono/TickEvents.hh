
#ifndef __HH_ROCKHOPPER_TICK_EVENTS_
#define __HH_ROCKHOPPER_TICK_EVENTS_

#include "RockHopper/Chrono/TickEvents.fwd"

#include "RockHopper/Event/Event.hh"

namespace RockHopper::Chrono
{

    struct TickEvent
        : Event::EventCategory<TickEventSet>
    {
    };

    struct TickEvent_Init
        : Event::EventListenable<TickEvent,TickEvent_Init>
    {
    };

    struct TickEvent_Dispose
        : Event::EventListenable<TickEvent,TickEvent_Dispose>
    {
    };

    struct TickEvent_OnTick
        : Event::EventListenable<TickEvent,TickEvent_OnTick>
    {
    };

} // namespace RockHopper::Chrono

#endif /* __HH_ROCKHOPPER_TICK_EVENTS_ */


#ifndef ROCKHOPPER_CHRONO_TICK_EVENTS_HH
#define ROCKHOPPER_CHRONO_TICK_EVENTS_HH

#include "RockHopper/Event/Event.hh"

#include <chrono>

namespace RockHopper::Chrono
{

    struct TickEvent_Init
    {
    };

    struct TickEvent_Dispose
    {
    };

    struct TickEvent_OnTick
    {
        using delta_t = std::chrono::system_clock::duration;
        delta_t const delta;
    };

    using TickEvent = Event::EventSet<
        TickEvent_Init,
        TickEvent_Dispose,
        TickEvent_OnTick >;

} // namespace RockHopper::Chrono

#endif /* ROCKHOPPER_CHRONO_TICK_EVENTS_HH */

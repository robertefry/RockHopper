
#ifndef ROCKHOPPER_EVENT_EVENT_EVENT_KIT_HH
#define ROCKHOPPER_EVENT_EVENT_EVENT_KIT_HH

#include "RockHopper/Event/Event.hh"

namespace RockHopper::Event
{

    template <typename... T_EventPack>
    template <typename T_Event>
    class EventSet<T_EventPack...>::EventKit
    {
    public:
        template <typename... T_Args>
        static auto Make(T_Args&&... args) -> Variant
        {
            return T_Event{std::forward<T_Args>(args)...};
        }
    };

} // namespace RockHopper::Event

#endif /* ROCKHOPPER_EVENT_EVENT_EVENT_KIT_HH */

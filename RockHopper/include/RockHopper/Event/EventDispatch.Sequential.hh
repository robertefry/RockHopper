
#ifndef ROCKHOPPER_EVENT_EVENT_DISPATCHER_SEQUENTIAL_HH
#define ROCKHOPPER_EVENT_EVENT_DISPATCHER_SEQUENTIAL_HH

#include "RockHopper/Event/Event.hh"

namespace RockHopper::Event::Dispatch
{

    class Sequential
    {
    public:
        template <typename T_EventSet, typename T_ListenerList, typename T_Event>
        void dispatch(T_ListenerList const& listeners, T_Event&& event)
        {
            using EventSet = T_EventSet;

            for (auto const& listener : listeners)
            {
                EventSet::Dispatch(*listener,event);
            }
        }
    };

} // namespace RockHopper::Event::Dispatch

#endif /* ROCKHOPPER_EVENT_EVENT_DISPATCHER_SEQUENTIAL_HH */

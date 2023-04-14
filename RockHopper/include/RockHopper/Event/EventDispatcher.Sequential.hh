
#ifndef ROCKHOPPER_EVENT_EVENT_DISPATCHER_SEQUENTIAL_HH
#define ROCKHOPPER_EVENT_EVENT_DISPATCHER_SEQUENTIAL_HH

#include "RockHopper/Event/Event.hh"

namespace RockHopper::Event::Dispatch
{

    template <typename T_EventSet>
    class Sequential
    {
        using EventSet = T_EventSet;

    public:
        template <typename T_ListenerList, typename T_Event>
        void dispatch(T_ListenerList const& listeners, T_Event&& event)
        {
            for (auto const& listener : listeners)
            {
                EventSet::Dispatch(*listener,std::forward<T_Event>(event));
            }
        }
    };

} // namespace RockHopper::Event::Dispatch

#endif /* ROCKHOPPER_EVENT_EVENT_DISPATCHER_SEQUENTIAL_HH */

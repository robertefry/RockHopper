
#ifndef __HH_ROCKHOPPER_EVENT_DISPATCH_
#define __HH_ROCKHOPPER_EVENT_DISPATCH_

namespace RockHopper::Event::Dispatch
{

    /*
     * Events are dispatched sequentially on the same thread as the dispatch
     * caller.
     */
    class SequentialDispatch
    {
    public:
        template <typename T_Event, typename T_ListenerList>
        void dispatch_event(
            T_ListenerList const& listeners,
            T_Event&& event )
        {
            for (auto listener : listeners)
            {
                event.accept(listener.get());
            }
        }
    };

} // namespace RockHopper::Event::Dispatch

#endif /* __HH_ROCKHOPPER_EVENT_DISPATCH_ */

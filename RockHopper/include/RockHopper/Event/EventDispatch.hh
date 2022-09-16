
#ifndef __HH_ROCKHOPPER_EVENT_DISPATCH_
#define __HH_ROCKHOPPER_EVENT_DISPATCH_

#include <future>
#include <latch>

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

    /*
     * Events are dispatched in parallel on non-reusable threads - one new
     * thread per event to be dispatched.
     */
    class ParallelDispatch
    {
    public:
        template <typename T_Event, typename T_ListenerList>
        void dispatch_event(
            T_ListenerList const& listeners,
            T_Event&& event )
        {
            std::latch latch {(std::ptrdiff_t)listeners.size()};

            for (auto listener : listeners)
            {
                (void) std::async([&, event=event ]()
                {
                    event.accept(listener.get());
                    latch.count_down();
                });
            }
            latch.wait();
        }
    };

} // namespace RockHopper::Event::Dispatch

#endif /* __HH_ROCKHOPPER_EVENT_DISPATCH_ */

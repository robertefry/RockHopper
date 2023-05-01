
#ifndef ROCKHOPPER_EVENT_EVENT_DISPATCHER_THREAD_POOL_HH
#define ROCKHOPPER_EVENT_EVENT_DISPATCHER_THREAD_POOL_HH

#include "RockHopper/Event/Event.hh"

#include "RockHopper/Util/ThreadPool.hh"

#include <vector>

namespace RockHopper::Event::Dispatch
{

    class ThreadPool
    {
    public:
        template <typename T_EventSet, typename T_ListenerList, typename T_Event>
        void dispatch(T_ListenerList const& listeners, T_Event&& event)
        {
            using EventSet = T_EventSet;

            std::vector<std::future<void>> future_list;
            future_list.reserve(listeners.size());

            for (auto const& listener : listeners)
            {
                auto future = m_ThreadPool.push_task([&]()
                {
                    EventSet::Dispatch(*listener,event);
                });
                future_list.push_back(std::move(future));
            }

            for (auto& future : future_list)
            {
                future.wait();
            }
        }

    private:
        Util::ThreadPool m_ThreadPool{};
    };

} // namespace RockHopper::Event::Dispatch

#endif /* ROCKHOPPER_EVENT_EVENT_DISPATCHER_THREAD_POOL_HH */


#ifndef __HH_ROCKHOPPER_EVENT_HANDLER_
#define __HH_ROCKHOPPER_EVENT_HANDLER_

#include "RockHopper/Event/EventListener.hh"

#include <unordered_set>
#include <mutex>

namespace RockHopper
{

    template <typename T_Event>
    class EventHandler
    {
    public:
        explicit EventHandler() = default;
        virtual ~EventHandler() = default;
    public:
        void insert_event_listener(EventListener<T_Event>* listener)
        {
            std::lock_guard<std::mutex> guard {m_EventListenersMutex};
            m_EventListeners.insert(listener);
        }
        void remove_event_listener(EventListener<T_Event>* listener)
        {
            std::lock_guard<std::mutex> guard {m_EventListenersMutex};
            m_EventListeners.erase(listener);
        }
    protected:
        void dispatch_event(T_Event const& event) const
        {
            std::lock_guard<std::mutex> guard {m_EventListenersMutex};

            for (EventListener<T_Event>* listener : m_EventListeners)
            {
                listener->on_event(event);
            }
        }
    private:
        std::unordered_set<EventListener<T_Event>*> m_EventListeners{};
        mutable std::mutex m_EventListenersMutex{};
    };

}

#endif /* __HH_ROCKHOPPER_EVENT_HANDLER_ */

/**
 * @author Robert Fry
 * @date create 19-Jun-2021
 * @date modify 19-Jun-2021
 */

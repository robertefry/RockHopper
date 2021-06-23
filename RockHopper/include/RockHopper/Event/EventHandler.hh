
#ifndef __HH_ROCKHOPPER_EVENT_HANDLER_
#define __HH_ROCKHOPPER_EVENT_HANDLER_

#include "RockHopper/Event/Event.hh"
#include "RockHopper/Logging/Logger.hh"

#include <unordered_set>
#include <mutex>
#include <typeinfo>

namespace RockHopper
{

    template <typename T_EventListenable>
        requires IsEventListenable<T_EventListenable>
    class EventHandler
    {
        using ListenerType = T_EventListenable::ListenerType;
    public:
        explicit EventHandler() = default;
        virtual ~EventHandler() = default;
    public:
        void insert_event_listener(ListenerType* listener)
        {
            std::lock_guard<std::mutex> guard {m_EventListenersMutex};
            m_EventListeners.insert(listener);
            ROCKHOPPER_INTERNAL_LOG_DEBUG("inserted an event listener for events of type '{}'",typeid(T_EventListenable).name());
        }
        void remove_event_listener(ListenerType* listener)
        {
            std::lock_guard<std::mutex> guard {m_EventListenersMutex};
            m_EventListeners.erase(listener);
            ROCKHOPPER_INTERNAL_LOG_DEBUG("removed an event listener for events of type '{}'",typeid(T_EventListenable).name());
        }
    protected:
        void dispatch_event(T_EventListenable const& event) const
        {
            std::lock_guard<std::mutex> guard {m_EventListenersMutex};

            for (ListenerType* listener : m_EventListeners)
            {
                event.accept(listener);
            }
            ROCKHOPPER_INTERNAL_LOG_DEBUG("dispatched an event of type '{}'",typeid(T_EventListenable).name());
        }
    private:
        std::unordered_set<ListenerType*> m_EventListeners{};
        mutable std::mutex m_EventListenersMutex{};
    };

}

#endif /* __HH_ROCKHOPPER_EVENT_HANDLER_ */

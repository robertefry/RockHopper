
#ifndef __HH_ROCKHOPPER_EVENT_HANDLER_
#define __HH_ROCKHOPPER_EVENT_HANDLER_

#include "RockHopper/Event/EventListener.hh"

#include <unordered_set>
#include <mutex>

namespace RockHopper
{

    template <typename T_Event>
    class EventHandler_Base
    {
    public:
        explicit EventHandler_Base() = default;
        virtual ~EventHandler_Base() = default;
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

    template <typename... T_Events>
    class EventHandler
        : public virtual EventHandler_Base<T_Events>...
    {
    public:
        explicit EventHandler() = default;
        virtual ~EventHandler() = default;
    public:
        template <typename T_Event>
        void insert_event_listener(EventListener<T_Event>* listener)
        {
            EventHandler_Base<T_Event>::insert_event_listener(listener);
        }
        template <typename T_Event>
        void remove_event_listener(EventListener<T_Event>* listener)
        {
            EventHandler_Base<T_Event>::remove_event_listener(listener);
        }
    protected:
        template <typename T_Event>
        void dispatch_event(T_Event const& event) const
        {
            EventHandler_Base<T_Event>::dispatch_event(event);
        }
    };

}

#endif /* __HH_ROCKHOPPER_EVENT_HANDLER_ */

/**
 * @author Robert Fry
 * @date create 19-Jun-2021
 * @date modify 19-Jun-2021
 */

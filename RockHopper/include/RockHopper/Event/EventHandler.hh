
#ifndef __HH_ROCKHOPPER_EVENT_HANDLER_
#define __HH_ROCKHOPPER_EVENT_HANDLER_

#include <unordered_set>
#include <mutex>

namespace RockHopper
{

    template <typename T_Event>
    struct EventListener
    {
        virtual void on_event(T_Event const&) = 0;
    };

    template <typename T_Event>
    class EventHandler
    {
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

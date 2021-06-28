
#ifndef __HH_ROCKHOPPER_EVENT_HANDLER_
#define __HH_ROCKHOPPER_EVENT_HANDLER_

#include "RockHopper/Event/Event.hh"
#include "RockHopper/Logging/Logger.hh"

#include <unordered_set>
#include <memory>
#include <mutex>
#include <typeinfo>

/* ************************************************************************** */
// [Definition] RockHopper::EventHandler
/* ************************************************************************** */

namespace RockHopper
{

    template <typename T_EventCategory>
        requires IsEventCategory<T_EventCategory>
    class EventHandler
    {
        using ListenerType = T_EventCategory::ListenerType;

    public:
        virtual ~EventHandler() = default;
        explicit EventHandler() = default;

        explicit EventHandler(EventHandler&&);

    public:
        template <typename T_Derrived>
        void persist_event_listener(T_Derrived&& listener);

        void insert_event_listener(ListenerType* listener);
        void remove_event_listener(ListenerType* listener);

        void dispatch_event(T_EventCategory const& event) const;

    private:
        std::unordered_set<std::unique_ptr<ListenerType>> m_OwnedEventListeners{};
        std::unordered_set<ListenerType*> m_EventListeners{};
        mutable std::mutex m_EventListenersMutex{};
    };

} // namespace RockHopper

/* ************************************************************************** */
// [Implementation] RockHopper::EventHandler
/* ************************************************************************** */

namespace RockHopper
{

    template <typename T_EventCategory>
    EventHandler<T_EventCategory>::EventHandler(EventHandler&& other)
    {
        std::unique_lock lock_1 {m_EventListenersMutex,std::defer_lock};
        std::unique_lock lock_2 {other.m_EventListenersMutex,std::defer_lock};
        std::lock(lock_1,lock_2);

        m_OwnedEventListeners = std::move(other.m_OwnedEventListeners);
        other.m_OwnedEventListeners.clear();

        m_EventListeners = std::move(other.m_EventListeners);
        other.m_EventListeners.clear();
    }

    template <typename T_EventCategory>
    template <typename T_Derrived>
    void EventHandler<T_EventCategory>::persist_event_listener(T_Derrived&& listener)
    {
        std::lock_guard<std::mutex> guard {m_EventListenersMutex};
        auto owned = std::make_unique<T_Derrived>(std::move(listener));
        m_EventListeners.insert(owned.get());
        m_OwnedEventListeners.insert(std::move(owned));
        ROCKHOPPER_INTERNAL_LOG_DEBUG("persisting an event listener for events of type '{}'",typeid(T_EventCategory).name());
    }

    template <typename T_EventCategory>
    void EventHandler<T_EventCategory>::insert_event_listener(ListenerType* listener)
    {
        std::lock_guard<std::mutex> guard {m_EventListenersMutex};
        m_EventListeners.insert(listener);
        ROCKHOPPER_INTERNAL_LOG_DEBUG("inserted an event listener for events of type '{}'",typeid(T_EventCategory).name());
    }

    template <typename T_EventCategory>
    void EventHandler<T_EventCategory>::remove_event_listener(ListenerType* listener)
    {
        std::lock_guard<std::mutex> guard {m_EventListenersMutex};
        m_EventListeners.erase(listener);
        ROCKHOPPER_INTERNAL_LOG_DEBUG("removed an event listener for events of type '{}'",typeid(T_EventCategory).name());
    }

    template <typename T_EventCategory>
    void EventHandler<T_EventCategory>::dispatch_event(T_EventCategory const& event) const
    {
        std::lock_guard<std::mutex> guard {m_EventListenersMutex};

        for (ListenerType* listener : m_EventListeners)
        {
            event.accept(listener);
        }
        ROCKHOPPER_INTERNAL_LOG_TRACE("dispatched an event of type '{}'",typeid(T_EventCategory).name());
    }

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_EVENT_HANDLER_ */

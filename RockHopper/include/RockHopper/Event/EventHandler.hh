
#ifndef __HH_ROCKHOPPER_EVENT_HANDLER_
#define __HH_ROCKHOPPER_EVENT_HANDLER_

#include "RockHopper/Event/Event.hh"
#include "RockHopper/Logging/Logger.hh"

#include <unordered_set>
#include <memory>
#include <atomic>
#include <mutex>
#include <thread>
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
        void insert_event_listener(ListenerType* listener);
        void remove_event_listener(ListenerType* listener);

        template <typename T_EventListenable, typename... Args>
        void emplace_event_listener(Args const&...);
        template <typename T_EventListenable>
        void give_event_listener(T_EventListenable&& listener);
        template <typename T_EventListenable>
        void give_event_listener(T_EventListenable const& listener);
        template <typename T_EventListenable>
        void take_event_listener(T_EventListenable const& listener);

        void dispatch_event(T_EventCategory const& event) const;

        inline void set_parallel_dispatch(bool allow) { m_ParallelDispatch = allow; }
        inline bool get_parallel_dispatch() const { return m_ParallelDispatch; }

    private:
        std::unordered_set<std::unique_ptr<ListenerType>> m_OwnedEventListeners{};
        std::unordered_set<ListenerType*> m_EventListeners{};
        mutable std::mutex m_Mutex{};

        std::atomic<bool> m_ParallelDispatch = false;
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
        std::unique_lock lock_1 {m_Mutex,std::defer_lock};
        std::unique_lock lock_2 {other.m_Mutex,std::defer_lock};
        std::lock(lock_1,lock_2);

        m_OwnedEventListeners = std::move(other.m_OwnedEventListeners);
        other.m_OwnedEventListeners.clear();

        m_EventListeners = std::move(other.m_EventListeners);
        other.m_EventListeners.clear();
    }

    template <typename T_EventCategory>
    void EventHandler<T_EventCategory>::insert_event_listener(ListenerType* listener)
    {
        std::lock_guard<std::mutex> guard {m_Mutex};
        m_EventListeners.insert(listener);
        ROCKHOPPER_INTERNAL_LOG_TRACE("inserted an event listener for events of type '{}'",typeid(T_EventCategory).name());
    }

    template <typename T_EventCategory>
    void EventHandler<T_EventCategory>::remove_event_listener(ListenerType* listener)
    {
        std::lock_guard<std::mutex> guard {m_Mutex};
        m_EventListeners.erase(listener);
        ROCKHOPPER_INTERNAL_LOG_TRACE("removed an event listener for events of type '{}'",typeid(T_EventCategory).name());
    }

    template <typename T_EventCategory>
    template <typename T_EventListenable, typename... Args>
    void EventHandler<T_EventCategory>::emplace_event_listener(Args const&... args)
    {
        std::lock_guard<std::mutex> guard {m_Mutex};
        auto owned = std::make_unique<T_EventListenable>(std::forward<Args>(args)...);
        m_EventListeners.insert(owned.get());
        m_OwnedEventListeners.insert(std::move(owned));
        ROCKHOPPER_INTERNAL_LOG_TRACE("emplaced an event listener for events of type '{}'",typeid(T_EventListenable).name());
    }

    template <typename T_EventCategory>
    template <typename T_EventListenable>
    void EventHandler<T_EventCategory>::give_event_listener(T_EventListenable&& listener)
    {
        std::lock_guard<std::mutex> guard {m_Mutex};
        auto owned = std::make_unique<T_EventListenable>(std::move(listener));
        m_EventListeners.insert(owned.get());
        m_OwnedEventListeners.insert(std::move(owned));
        ROCKHOPPER_INTERNAL_LOG_TRACE("given an event listener for events of type '{}'",typeid(T_EventListenable).name());
    }

    template <typename T_EventCategory>
    template <typename T_EventListenable>
    void EventHandler<T_EventCategory>::give_event_listener(T_EventListenable const& listener)
    {
        std::lock_guard<std::mutex> guard {m_Mutex};
        auto owned = std::make_unique<T_EventListenable>(listener);
        m_EventListeners.insert(owned.get());
        m_OwnedEventListeners.insert(std::move(owned));
        ROCKHOPPER_INTERNAL_LOG_TRACE("given an event listener for events of type '{}'",typeid(T_EventListenable).name());
    }

    template <typename T_EventCategory>
    template <typename T_EventListenable>
    void EventHandler<T_EventCategory>::take_event_listener(T_EventListenable const& listener)
    {
        std::lock_guard<std::mutex> guard {m_Mutex};
        auto itr = m_OwnedEventListeners.find(listener);
        m_EventListeners.erase(itr->get());
        m_OwnedEventListeners.erase(listener);
        ROCKHOPPER_INTERNAL_LOG_TRACE("taken an event listener for events of type '{}'",typeid(T_EventListenable).name());
    }

    template <typename T_EventCategory>
    void EventHandler<T_EventCategory>::dispatch_event(T_EventCategory const& event) const
    {
        std::lock_guard<std::mutex> guard {m_Mutex};

        if (m_ParallelDispatch)
        {
            std::vector<std::thread> dispatch_threads;

            for (ListenerType* listener : m_EventListeners)
            {
                dispatch_threads.emplace_back([&,listener]()
                {
                    event.accept(listener);
                });
            }
            for (std::thread& thread : dispatch_threads)
            {
                thread.join();
            }
            ROCKHOPPER_INTERNAL_LOG_TRACE("parallel-dispatched an event of type '{}'",typeid(T_EventCategory).name());
        }
        else
        {
            for (ListenerType* listener : m_EventListeners)
            {
                event.accept(listener);
            }
            ROCKHOPPER_INTERNAL_LOG_TRACE("dispatched an event of type '{}'",typeid(T_EventCategory).name());
        }
    }

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_EVENT_HANDLER_ */

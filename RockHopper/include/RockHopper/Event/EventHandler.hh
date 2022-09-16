
#ifndef __HH_ROCKHOPPER_EVENT_HANDLER_
#define __HH_ROCKHOPPER_EVENT_HANDLER_

#include "RockHopper/Event/Event.hh"
#include "RockHopper/Event/EventDispatch.hh"

#include <unordered_set>
#include <memory>
#include <mutex>

/* ************************************************************************** */
// [Definition] RockHopper::Event::EventHandler
/* ************************************************************************** */

namespace RockHopper::Event
{

    template <typename T_EventCategory>
    struct I_EventHandler
        : protected virtual T_EventCategory::ListenerType
    {
        using EventCategory = T_EventCategory;
        using ListenerType = EventCategory::ListenerType;
        using ListenerPtr = std::shared_ptr<ListenerType>;

        virtual ~I_EventHandler() = default;

    public:
        virtual void insert_event_listener(ListenerPtr listener) = 0;
        virtual void remove_event_listener(ListenerPtr listener) = 0;
    };

    template <typename T_EventCategory, typename T_EventDispatcher>
    class EventHandler
        : public virtual I_EventHandler<T_EventCategory>
    {
    public:
        using EventCategory = T_EventCategory;
        using EventDispatcher = T_EventDispatcher;
        using ListenerType = EventCategory::ListenerType;
        using ListenerPtr = std::shared_ptr<ListenerType>;
        using ListenerList = std::unordered_set<ListenerPtr>;

        virtual ~EventHandler() = default;
        explicit EventHandler() = default;

        EventHandler(EventHandler const&);
        EventHandler& operator=(EventHandler const&);

        EventHandler(EventHandler&&);
        EventHandler& operator=(EventHandler&&);

    public:
        virtual void insert_event_listener(ListenerPtr listener) override;
        virtual void remove_event_listener(ListenerPtr listener) override;

    protected:
        template <typename T_Event>
        void dispatch_event(T_Event&& event);

    private:
        std::unique_ptr<EventDispatcher> m_Dispatcher = std::make_unique<EventDispatcher>();

        ListenerList m_ListenerList{};
        mutable std::mutex m_ListenerMutex{}; // prevent dispatch while changing listeners
    };

} // namespace RockHopper::Event

/* ************************************************************************** */
// [Implementation] RockHopper::Event::EventHandler
/* ************************************************************************** */

namespace RockHopper::Event
{

    template <typename T_EventCategory, typename T_EventDispatcher>
    EventHandler<T_EventCategory,T_EventDispatcher>::EventHandler(EventHandler const& other)
    {
        *this = other;
    }

    template <typename T_EventCategory, typename T_EventDispatcher>
    EventHandler<T_EventCategory,T_EventDispatcher>&
    EventHandler<T_EventCategory,T_EventDispatcher>::operator=(EventHandler const& other)
    {
        std::unique_lock lock_1 {m_ListenerMutex,std::defer_lock};
        std::unique_lock lock_2 {other.m_ListenerMutex,std::defer_lock};
        std::lock(lock_1,lock_2);

        m_Dispatcher = other.m_Dispatcher;
        m_ListenerList = other.m_ListenerList;

        return *this;
    }

    template <typename T_EventCategory, typename T_EventDispatcher>
    EventHandler<T_EventCategory,T_EventDispatcher>::EventHandler(EventHandler&& other)
    {
        *this = std::move(other);
    }

    template <typename T_EventCategory, typename T_EventDispatcher>
    EventHandler<T_EventCategory,T_EventDispatcher>&
    EventHandler<T_EventCategory,T_EventDispatcher>::operator=(EventHandler&& other)
    {
        std::unique_lock lock_1 {m_ListenerMutex,std::defer_lock};
        std::unique_lock lock_2 {other.m_ListenerMutex,std::defer_lock};
        std::lock(lock_1,lock_2);

        std::swap(m_Dispatcher,other.m_Dispatcher);
        std::swap(m_ListenerList,other.m_ListenerList);

        return *this;
    }

    template <typename T_EventCategory, typename T_EventDispatcher>
    void EventHandler<T_EventCategory,T_EventDispatcher>::insert_event_listener(ListenerPtr listener)
    {
        std::unique_lock lock {m_ListenerMutex};
        m_ListenerList.insert(std::move(listener));
    }

    template <typename T_EventCategory, typename T_EventDispatcher>
    void EventHandler<T_EventCategory,T_EventDispatcher>::remove_event_listener(ListenerPtr listener)
    {
        std::unique_lock lock {m_ListenerMutex};
        m_ListenerList.erase(std::move(listener));
    }

    template <typename T_EventCategory, typename T_EventDispatcher>
    template <typename T_Event>
    void EventHandler<T_EventCategory,T_EventDispatcher>::dispatch_event(T_Event&& event)
    {
        std::unique_lock lock {m_ListenerMutex};
        event.accept(this);
        m_Dispatcher->dispatch_event(m_ListenerList,std::move(event));
    }

} // namespace RockHopper::Event

#endif /* __HH_ROCKHOPPER_EVENT_HANDLER_ */

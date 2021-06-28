
#ifndef __HH_ROCKHOPPER_EVENT_LISTENER_
#define __HH_ROCKHOPPER_EVENT_LISTENER_

#include "RockHopper/Event/Event.hh"

#include <functional>
#include <mutex>
#include <condition_variable>

namespace RockHopper
{

    template <typename T_EventCategory, typename T_Event>
        requires IsEventListenable<T_EventCategory,T_Event>
    class EventFunctionListener
        : public T_EventCategory::ListenerType
    {
        using Function = std::function<void(T_Event const&)>;
    public:
        explicit EventFunctionListener(Function const& func)
            : m_OnEventFunction{func}
        {}
        virtual void on_event(T_Event const& event) override
        {
            m_OnEventFunction(event);
        }
    private:
        Function m_OnEventFunction;
    };

    template <typename T_EventCategory>
        requires IsEventCategory<T_EventCategory>
    class EventWaitListener
        : public T_EventCategory::ListenerType
    {
    public:
        enum EventNotifyType { NOTIFY_ALL, NOTIFY_ONE, NOTIFY_NONE };
        explicit EventWaitListener() = default;
        virtual ~EventWaitListener() = default;
    public:
        virtual EventNotifyType get_notify_type(T_EventCategory const&)
        {
            return NOTIFY_ALL;
        }
        void wait() const
        {
            std::unique_lock<std::mutex> lock {m_Mutex};
            m_NotifyVariable.wait(lock);
        }
    private:
        virtual void on_event(T_EventCategory const& event) override final
        {
            std::unique_lock<std::mutex> lock {m_Mutex};
            switch (get_notify_type(event))
            {
                case NOTIFY_ALL: m_NotifyVariable.notify_all(); break;
                case NOTIFY_ONE: m_NotifyVariable.notify_one(); break;
                default: break;
            }
        }
    private:
        mutable std::mutex m_Mutex;
        mutable std::condition_variable m_NotifyVariable;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_EVENT_LISTENER_ */

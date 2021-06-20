
#ifndef __HH_ROCKHOPPER_EVENT_LISTENER_
#define __HH_ROCKHOPPER_EVENT_LISTENER_

#include <mutex>
#include <condition_variable>

namespace RockHopper
{

    template <typename T_Event>
    struct EventListener
    {
        virtual void on_event(T_Event const&) = 0;
    };

    template <typename T_Event>
    class EventWaitListener
        : public EventListener<T_Event>
    {
    public:
        enum EventNotifyType { NOTIFY_ALL, NOTIFY_ONE, NOTIFY_NONE };
        explicit EventWaitListener() = default;
        virtual ~EventWaitListener() = default;
    public:
        virtual EventNotifyType get_notify_type(T_Event const&)
        {
            return NOTIFY_ALL;
        }
        void wait() const
        {
            std::unique_lock<std::mutex> lock {m_Mutex};
            m_NotifyVariable.wait(lock);
        }
    private:
        virtual void on_event(T_Event const& event) override final
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

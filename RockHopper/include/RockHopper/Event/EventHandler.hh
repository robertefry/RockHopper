
#ifndef ROCKHOPPER_EVENT_EVENT_HANDLER_HH
#define ROCKHOPPER_EVENT_EVENT_HANDLER_HH

#include <memory>
#include <vector>

namespace RockHopper::Event
{

    template <typename T_EventSet, typename T_Dispatcher>
    class EventHandler
        : protected T_EventSet::Listener
    {
        using Dispatcher = T_Dispatcher;
        using EventSet = T_EventSet;
        using Variant = typename EventSet::Variant;
        using Listener = typename EventSet::Listener;

    public:
        explicit EventHandler(Dispatcher dispatcher = {})
            : m_Dispatcher{std::move(dispatcher)}
        {}

        template <typename T_Listener>
            requires std::is_base_of<Listener,std::remove_cvref_t<T_Listener>>::value
        void emplace_event_listener(T_Listener&& listener)
        {
            insert_event_listener(std::make_shared<T_Listener>(std::forward<T_Listener>(listener)));
        }

        void insert_event_listener(std::shared_ptr<Listener> ptr)
        {
            m_ListenerList.push_back(std::move(ptr));
        }

        void remove_event_listener(std::shared_ptr<Listener> const& ptr)
        {
            std::erase(m_ListenerList.begin(),m_ListenerList.end(),ptr);
        }

    protected:
        template <typename T_Event>
        void dispatch_event(T_Event&& event)
        {
            this->on_event(event);
            m_Dispatcher.template dispatch<EventSet>(m_ListenerList,std::forward<T_Event>(event));
        }

    private:
        Dispatcher mutable m_Dispatcher;
        std::vector<std::shared_ptr<Listener>> m_ListenerList{};
    };

} // namespace RockHopper::Event

#endif /* ROCKHOPPER_EVENT_EVENT_HANDLER_HH */

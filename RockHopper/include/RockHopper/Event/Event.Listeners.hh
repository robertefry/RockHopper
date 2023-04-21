
#ifndef ROCKHOPPER_EVENT_EVENT_LISTENER_KIT_HH
#define ROCKHOPPER_EVENT_EVENT_LISTENER_KIT_HH

#include "RockHopper/Event/Event.hh"

namespace RockHopper::Event
{

    template <typename... T_EventPack>
    template <typename T_Event>
    class EventSet<T_EventPack...>::ListenerKit
    {
    public:
        template <typename T_OnEvent>
        class OnEventListener : public Listener
        {
        public:
            explicit OnEventListener(T_OnEvent func)
                : m_OnEvent{std::move(func)}
            {}

            using Listener::on_event;
            void on_event(T_Event const& event) override { m_OnEvent(event); }
            void on_event(T_Event & event) override { m_OnEvent(event); }

        private:
            T_OnEvent m_OnEvent;
        };

        template <typename T_OnEvent>
        [[nodiscard]] static auto Make(T_OnEvent&& func) -> OnEventListener<T_OnEvent>
        {
            return OnEventListener<T_OnEvent>{std::forward<T_OnEvent>(func)};
        }
    };

} // namespace RockHopper::Event

#endif /* ROCKHOPPER_EVENT_EVENT_LISTENER_KIT_HH */

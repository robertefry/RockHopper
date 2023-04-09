
#ifndef ROCKHOPPER_EVENT_EVENT_HH
#define ROCKHOPPER_EVENT_EVENT_HH

#include <variant>

namespace RockHopper::Event
{

    template <typename... T_EventPack>
    class EventSet
    {
        template <typename T_Event>
        struct I_Listener_Base
        {
            virtual ~I_Listener_Base() = default;

            virtual void on_event(T_Event const&) {}
            virtual void on_event(T_Event &) {}
            virtual void on_event(T_Event const&&) {}
            virtual void on_event(T_Event &&) {}
        };

        struct I_Listener : virtual I_Listener_Base<T_EventPack>...
        {
            using I_Listener_Base<T_EventPack>::on_event...;
        };

    public:
        using Variant = std::variant<T_EventPack...>;
        using Listener = I_Listener;

        template <typename T_Event>
        class EventKit;

        template <typename T_Event>
        class ListenerKit;

        template <typename T_Listener, typename T_Event>
        static void Dispatch(T_Listener&& listener, T_Event&& event)
        {
            auto const visitor = [&]<typename E>(E&& e) { listener.on_event(std::forward<E>(e)); };
            std::visit(visitor,std::forward<T_Event>(event));
        }
    };

    template <typename... T_EventPack>
    template <typename T_Event>
    class EventSet<T_EventPack...>::EventKit
    {
    public:
        template <typename... T_Args>
        static auto Make(T_Args&&... args) -> Variant
        {
            return T_Event{std::forward<T_Args>(args)...};
        }
    };

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
            void on_event(T_Event const&& event) override { m_OnEvent(std::move(event)); }
            void on_event(T_Event && event) override { m_OnEvent(std::move(event)); }

        private:
            T_OnEvent m_OnEvent;
        };

        template <typename T_OnEvent>
        static auto Make(T_OnEvent&& func) -> OnEventListener<T_OnEvent>
        {
            return OnEventListener<T_OnEvent>{std::forward<T_OnEvent>(func)};
        }
    };

} // namespace RockHopper::Event

#endif /* ROCKHOPPER_EVENT_EVENT_HH */

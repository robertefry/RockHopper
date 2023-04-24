
#ifndef ROCKHOPPER_EVENT_EVENT_HH
#define ROCKHOPPER_EVENT_EVENT_HH

#include <utility>
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

            virtual void on_event(T_Event const&) { /* do nothing by default */ }
            virtual void on_event(T_Event & event) { on_event(std::as_const(event)); }
        };

        struct I_Listener : I_Listener_Base<T_EventPack>...
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
            requires std::is_same<Variant,std::remove_cvref_t<T_Event>>::value
        static void Dispatch(T_Listener&& listener, T_Event&& event)
        {
            auto const visitor = [&]<typename E>(E&& e) { listener.on_event(std::forward<E>(e)); };
            std::visit(visitor,std::forward<T_Event>(event));
        }

        template <typename T_Listener, typename T_Event>
            requires std::disjunction<std::is_same<T_EventPack,std::remove_cvref_t<T_Event>>...>::value
        static void Dispatch(T_Listener&& listener, T_Event&& event)
        {
            listener.on_event(std::forward<T_Event>(event));
        }
    };

} // namespace RockHopper::Event

#endif /* ROCKHOPPER_EVENT_EVENT_HH */

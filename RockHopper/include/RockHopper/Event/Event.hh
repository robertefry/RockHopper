
#ifndef __HH_ROCKHOPPER_EVENT_
#define __HH_ROCKHOPPER_EVENT_

#include <type_traits>

namespace RockHopper
{

    template <typename T_Event>
    struct I_EventListener_Base
    {
        virtual ~I_EventListener_Base() = default;
        virtual void on_event(T_Event const&) {};
    };

    template <typename... T_Events>
    struct I_EventListener : I_EventListener_Base<T_Events>...
    {
    };

    template <typename... T_Events>
    struct EventSet
    {
        using ListenerType = I_EventListener<T_Events...>;
    };

    template <typename T_EventSet>
    struct I_EventListenable
    {
        using EventSet = T_EventSet;
        using ListenerType = T_EventSet::ListenerType;

        virtual ~I_EventListenable() = default;
        virtual void accept(ListenerType*) const = 0;
    };

    template <typename T_Event, typename T_EventListenable>
    struct EventListenable : virtual T_EventListenable
    {
        using EventSet = typename T_EventListenable::EventSet;
        using ListenerType = typename T_EventListenable::ListenerType;

        virtual ~EventListenable() = default;

        virtual void accept(ListenerType* listener) const override
        {
            static_cast<I_EventListener_Base<T_Event>*>(listener)
                ->on_event(static_cast<T_Event const&>(*this));
        }
    };

    template <typename T_EventListenable>
    concept IsEventListenable = std::is_base_of<
        I_EventListenable<typename T_EventListenable::EventSet>,T_EventListenable >::value;

    template <typename T_EventListenable, typename T_Event>
    concept IsSubEventListenable = IsEventListenable<T_EventListenable>
        && std::is_base_of<T_EventListenable,T_Event>::value;

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_EVENT_ */

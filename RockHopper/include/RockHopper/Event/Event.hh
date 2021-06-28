
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
    struct EventCategory
    {
        using EventSet = T_EventSet;
        using ListenerType = T_EventSet::ListenerType;

        virtual ~EventCategory() = default;
        virtual void accept(ListenerType*) const = 0;
    };

    template <typename T_Event, typename T_EventCategory>
    struct EventListenable : virtual T_EventCategory
    {
        using EventCategory = T_EventCategory;
        using ListenerType = T_EventCategory::ListenerType;

        virtual ~EventListenable() = default;

        virtual void accept(ListenerType* listener) const override
        {
            static_cast<I_EventListener_Base<T_Event>*>(listener)
                ->on_event(static_cast<T_Event const&>(*this));
        }
    };

    template <typename T_EventCategory>
    concept IsEventCategory = std::is_base_of<
        EventCategory<typename T_EventCategory::EventSet>,T_EventCategory >::value;

    template <typename T_Event, typename T_EventCategory = T_Event::EventCategory>
    concept IsEventListenable = IsEventCategory<T_EventCategory>
        && std::is_base_of<T_EventCategory,T_Event>::value;

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_EVENT_ */


#ifndef __HH_ROCKHOPPER_EVENT_
#define __HH_ROCKHOPPER_EVENT_

namespace RockHopper::Event
{

    template <typename T_Event>
    struct I_EventListener_Base
    {
        virtual ~I_EventListener_Base() = default;
        virtual void on_event(T_Event const&) {};
    };

    template <typename T_EventCategory, typename... T_Events>
    struct I_EventListener : I_EventListener_Base<T_Events>...
    {
        virtual void on_event_category(T_EventCategory const&) {};
    };

    // TODO: Add support for const and mutable listeners

    template <typename T_EventCategory, typename... T_Events>
    struct EventSet
    {
        using ListenerType = I_EventListener<T_EventCategory,T_Events...>;
    };

    template <typename T_EventSet>
    struct EventCategory
    {
        using EventSet = T_EventSet;
        using ListenerType = EventSet::ListenerType;

        virtual ~EventCategory() = default;
        virtual void accept(ListenerType*) const = 0;
    };

    template <typename T_EventCategory, typename T_Event>
    struct EventListenable : T_EventCategory
    {
        using EventCategory = T_EventCategory;
        using ListenerType = EventCategory::ListenerType;

        virtual ~EventListenable() = default;

        virtual void accept(ListenerType* listener) const override
        {
            static_cast<I_EventListener_Base<T_Event>*>(listener)
                ->on_event(static_cast<T_Event const&>(*this));
            listener->on_event_category(*this);
        }
    };

} // namespace RockHopper::Event

#endif /* __HH_ROCKHOPPER_EVENT_ */

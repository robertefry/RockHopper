
#ifndef __HH_ROCKHOPPER_LAYER_
#define __HH_ROCKHOPPER_LAYER_

#include "RockHopper/Event/Event.hh"
#include "RockHopper/Event/EventHandler.hh"

#include <map>

namespace RockHopper
{

    template <typename T_EventCategory>
        requires IsEventCategory<T_EventCategory>
    class Layer_Base
        : public T_EventCategory::ListenerType
    {
    public:
        auto handler(size_t prio) -> EventHandler<T_EventCategory>&
        {
            return m_EventHandlerMap[prio];
        }
        auto handler(size_t prio) const -> EventHandler<T_EventCategory> const&
        {
            return m_EventHandlerMap[prio];
        }
        void dispatch_event(T_EventCategory const& event) const
        {
            for (auto& [prio,handler] : m_EventHandlerMap)
            {
                handler.dispatch_event(event);
            }
        }
    protected:
        virtual void on_event_category(T_EventCategory const& event) override
        {
            dispatch_event(event);
        }
    private:
        std::map<size_t,EventHandler<T_EventCategory>> m_EventHandlerMap{};
    };

    template <typename... T_EventCategories>
    class Layer
        : public Layer_Base<T_EventCategories>...
    {
    public:
        template <typename T_EventCategory>
            requires std::disjunction<std::is_same<T_EventCategory,T_EventCategories>...>::value
        auto handler(size_t prio) -> EventHandler<T_EventCategory>&
        {
            return Layer_Base<T_EventCategory>::handler(prio);
        }
        template <typename T_EventCategory>
            requires std::disjunction<std::is_same<T_EventCategory,T_EventCategories>...>::value
        auto handler(size_t prio) const -> EventHandler<T_EventCategory> const&
        {
            return Layer_Base<T_EventCategory>::handler(prio);
        }
        template <typename T_EventCategory>
            requires std::disjunction<std::is_same<T_EventCategory,T_EventCategories>...>::value
        void dispatch_event(T_EventCategory const& event)
        {
            Layer_Base<T_EventCategory>::dispatch_event(event);
        }
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_LAYER_ */

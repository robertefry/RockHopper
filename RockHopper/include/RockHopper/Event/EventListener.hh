
#ifndef __HH_ROCKHOPPER_EVENT_LISTENER_
#define __HH_ROCKHOPPER_EVENT_LISTENER_

#include "RockHopper/Event/Event.hh"

#include <functional>

namespace RockHopper::Event
{

    template <typename T_Event>
    class EventListener
        : public virtual T_Event::ListenerType
    {
    public:
        using OnEventFunction = std::function<void(T_Event const&)>;

        virtual ~EventListener() = default;

        explicit EventListener(OnEventFunction const& func)
            : m_OnEventFunction{func}
        {}

        virtual void on_event(T_Event const& event) override
        {
            m_OnEventFunction(event);
        }

    private:
        OnEventFunction m_OnEventFunction;
    };

    template <typename T_Event>
    auto MakeListener(typename EventListener<T_Event>::OnEventFunction const& func)
    {
        return std::make_shared<EventListener<T_Event>>(func);
    }

    template <typename T_Event>
    auto MakeListener(typename EventListener<T_Event>::OnEventFunction&& func)
    {
        return std::make_shared<EventListener<T_Event>>(std::move(func));
    }

} // namespace RockHopper::Event

#endif /* __HH_ROCKHOPPER_EVENT_LISTENER_ */

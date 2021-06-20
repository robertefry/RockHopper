
#ifndef __HH_ROCKHOPPER_EVENT_LISTENER_
#define __HH_ROCKHOPPER_EVENT_LISTENER_

namespace RockHopper
{

    template <typename T_Event>
    struct EventListener
    {
        virtual void on_event(T_Event const&) = 0;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_EVENT_LISTENER_ */

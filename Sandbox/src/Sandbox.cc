
#include "Sandbox.hh"

#include "RockHopper/Logging/Logger.hh"
#include "RockHopper/Event/EventHandler.hh"

#include <iostream>

Sandbox::Sandbox()
{
}

Sandbox::~Sandbox()
{
}

void Sandbox::run()
{

    class WindowEvent
    {
    };

    class Window
        : public RockHopper::EventHandler<WindowEvent>
    {
    public:
        void trigger_event() const
        {
            WindowEvent event;
            dispatch_event(event);
        }
    };

    struct WindowManager
        : public RockHopper::EventListener<WindowEvent>
    {
        Window window;
    public:
        WindowManager()
        {
            window.insert_event_listener(this);
        }
    private:
        void on_event(WindowEvent const& event) override
        {
            ROCKHOPPER_LOG_INFO(__PRETTY_FUNCTION__);
        }
    };

    WindowManager window_manager;
    window_manager.window.trigger_event();
}

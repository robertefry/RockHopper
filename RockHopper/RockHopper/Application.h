
#pragma once

namespace RockHopper
{
    class Application
    {
    public:
        explicit Application();
        virtual ~Application();

        virtual void run();
    };

    // To be implemented by client!
    extern Application* CreateApplication();
}

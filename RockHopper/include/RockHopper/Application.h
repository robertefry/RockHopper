
#pragma once

namespace RockHopper
{
    class Application
    {
    public:
        explicit Application();
        virtual ~Application();

        virtual void run() = 0;
    };

    // To be implemented by the client!
    extern Application* CreateApplication();
}

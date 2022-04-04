
#ifndef __HH_ROCKHOPPER_APPLICATION_
#define __HH_ROCKHOPPER_APPLICATION_

namespace RockHopper
{
    class Application
    {
    public:
        explicit Application();
        virtual ~Application();

        virtual int run() = 0;
    };

    // To be implemented by the client!
    extern Application* CreateApplication();
}

#endif /* __HH_ROCKHOPPER_APPLICATION_ */

/**
 * @author Robert Fry
 * @date create 19-Jun-2021
 * @date modify 19-Jun-2021
 */

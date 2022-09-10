
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
}

#endif /* __HH_ROCKHOPPER_APPLICATION_ */

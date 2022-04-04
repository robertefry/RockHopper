
#include "RockHopper/RockHopper.hh"
#include "RockHopper/Logging/Logger.hh"

int main(int, const char**)
{
    RockHopper::Application* application = RockHopper::CreateApplication();
    ROCKHOPPER_INTERNAL_LOG_INFO("starting RockHopper client...");
    int ret = application->run();
    delete application;
    return ret;
}

/**
 * @author Robert Fry
 * @date create 19-Jun-2021
 * @date modify 19-Jun-2021
 */

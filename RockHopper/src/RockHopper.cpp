
#include "RockHopper/RockHopper.h"
#include "RockHopper/Logging/Logger.h"

int main(int, const char**)
{
    RockHopper::Application* application = RockHopper::CreateApplication();
    ROCKHOPPER_LOGINTERNAL_INFO("Starting RockHopper client...");
    application->run();
    delete application;
}

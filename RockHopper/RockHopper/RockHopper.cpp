
#include "RockHopper.h"

#include "Logging/Logger.h"

int main(int, const char**)
{
    RockHopper::Application* application = RockHopper::CreateApplication();
    ROCKHOPPER_LOGINTERNAL_INFO("Starting RockHopper application...");
    application->run();
    delete application;
}

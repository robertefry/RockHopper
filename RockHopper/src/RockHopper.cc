
#include "RockHopper/RockHopper.hh"
#include "RockHopper/Logging/Logger.hh"

int main(int, const char**)
{
    RockHopper::Application* application = RockHopper::CreateApplication();
    ROCKHOPPER_LOGINTERNAL_INFO("Starting RockHopper client...");
    application->run();
    delete application;
}

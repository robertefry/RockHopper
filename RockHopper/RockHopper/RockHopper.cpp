
#include "RockHopper.h"

int main(int, const char**)
{
    RockHopper::Application* application = RockHopper::CreateApplication();
    application->run();
    delete application;
}

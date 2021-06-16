
#include "RockHopper/RockHopper.h"
#include "Sandbox.h"

/*
 * main function implemented in RockHopper.cpp
 */

// Required by RockHopper engine
RockHopper::Application* RockHopper::CreateApplication()
{
    return new Sandbox{};
}

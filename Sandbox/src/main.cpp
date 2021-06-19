
#include "RockHopper/RockHopper.hh"
#include "Sandbox.hh"

/*
 * main function implemented in RockHopper.cpp
 */

// Required by RockHopper engine
RockHopper::Application* RockHopper::CreateApplication()
{
    return new Sandbox{};
}


#include "Sandbox.h"

#include "RockHopper/Logging/Logger.h"

#include <iostream>

Sandbox::Sandbox()
{
}

Sandbox::~Sandbox()
{
}

void Sandbox::run()
{
    ROCKHOPPER_LOG_INFO("Hello, World!");
}

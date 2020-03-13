
#include "Sandbox.h"

#include <iostream>

Sandbox::Sandbox()
{
}

Sandbox::~Sandbox()
{
}

void Sandbox::run()
{
    RockHopper::Application::run();
    std::cout << "Hello, World!" << std::endl;
}

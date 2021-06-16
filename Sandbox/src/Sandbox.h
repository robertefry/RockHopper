
#pragma once

#include "RockHopper/Application.h"

class Sandbox : public RockHopper::Application
{
public:
    Sandbox();
    ~Sandbox();

    virtual void run() override;
};

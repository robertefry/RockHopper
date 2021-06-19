
#pragma once

#include "RockHopper/Application.hh"

class Sandbox : public RockHopper::Application
{
public:
    Sandbox();
    ~Sandbox();

    virtual void run() override;
};

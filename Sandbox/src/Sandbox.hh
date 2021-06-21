
#pragma once

#include "RockHopper/Application.hh"
#include "RockHopper/Window/Window.hh"

class Sandbox : public RockHopper::Application
{
public:
    Sandbox();
    ~Sandbox();

    virtual void run() override;

private:
    RockHopper::Window m_Window;
};

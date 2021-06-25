
#pragma once

#include "RockHopper/Application.hh"
#include "RockHopper/Window/Window.hh"
#include "RockHopper/Input/Keyboard/Keyboard.hh"
#include "RockHopper/Input/Mouse/Mouse.hh"

class Sandbox : public RockHopper::Application
{
public:
    Sandbox();
    ~Sandbox();

    virtual void run() override;

private:
    RockHopper::Window m_Window;
    RockHopper::Keyboard m_Keyboard;
    RockHopper::Mouse m_Mouse;
};

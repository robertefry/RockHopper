
#pragma once

#include "RockHopper/Application.hh"
#include "RockHopper/Layer/Layer.hh"
#include "RockHopper/Window/Window.hh"
#include "RockHopper/Input/Keyboard/Keyboard.hh"
#include "RockHopper/Input/Mouse/Mouse.hh"

using namespace RockHopper;

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

    RockHopper::Layer<KeyEvent,MouseEvent> m_InputLayer{};
};

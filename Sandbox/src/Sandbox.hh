
#pragma once

#include "RockHopper/Application.hh"

#include "RockHopper/Engine/Engine.hh"
#include "RockHopper/Window/Window.hh"
#include "RockHopper/Input/Keyboard/Keyboard.hh"
#include "RockHopper/Input/Mouse/Mouse.hh"

#include "Examples/Cube.hh"

#include <memory>

using namespace RockHopper;

class Sandbox : public RockHopper::Application
{
public:
    Sandbox();
    ~Sandbox();

    virtual void run() override;

private:
    std::unique_ptr<Engine> m_Engine;
    std::unique_ptr<Window> m_Window;

    std::unique_ptr<Keyboard> m_Keyboard;
    std::unique_ptr<Mouse> m_Mouse;

    std::unique_ptr<Cube> m_Cube = std::make_unique<Cube>();
};

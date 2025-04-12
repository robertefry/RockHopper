
#pragma once

#include "RockHopper/Event/Layer.hh"
#include "RockHopper/Window/WindowEvents.hh"
#include "RockHopper/Input/Keyboard/Keyboard.hh"
#include "RockHopper/Input/Mouse/Mouse.hh"

#include <array>
#include <mutex>

using namespace RockHopper;

class Background
    : public Layer<WindowEvent,KeyEvent,MouseEvent>
{
public:
    virtual ~Background();
    explicit Background();

private:
    virtual void on_event(WindowRefreshEvent const&) override;
    virtual void on_event(KeyPressEvent const&) override;
    virtual void on_event(MousePressEvent const&) override;
    virtual void on_event(MouseDragEvent const&) override;

private:
    std::mutex m_Mutex{};
    std::array<float,4> m_ClearColor;
};

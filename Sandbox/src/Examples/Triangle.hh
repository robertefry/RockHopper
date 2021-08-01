
#pragma once

#include "RockHopper/Event/Layer.hh"
#include "RockHopper/Window/WindowEvents.hh"
#include "RockHopper/Input/Keyboard/KeyEvents.hh"

#include "RockHopper/Rendering/Mesh.hh"
#include "RockHopper/Rendering/Shader.hh"

#include <memory>

using namespace RockHopper;

class Triangle
    : public Layer<WindowEvent,KeyEvent>
{
public:
    virtual ~Triangle();
    explicit Triangle();

private:
    virtual void on_event(WindowInitEvent const&) override;
    virtual void on_event(WindowDisposeEvent const&) override;
    virtual void on_event(WindowRefreshEvent const&) override;

    virtual void on_event(KeyPressEvent const&) override;

private:
    std::unique_ptr<Mesh> m_Mesh{};
    std::unique_ptr<Shader> m_Shader{};
    float m_Scale = 1.0f;
};

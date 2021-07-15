
#pragma once

#include "RockHopper/Layer/Layer.hh"
#include "RockHopper/Window/WindowEvents.hh"

#include "RockHopper/Rendering/Mesh.hh"
#include "RockHopper/Rendering/Shader.hh"

#include <memory>

using namespace RockHopper;

class Triangle
    : public Layer<WindowEvent>
{
public:
    virtual ~Triangle();
    explicit Triangle();

private:
    virtual void on_event(WindowInitEvent const&) override;
    virtual void on_event(WindowDisposeEvent const&) override;
    virtual void on_event(WindowRefreshEvent const&) override;

private:
    std::unique_ptr<Mesh> m_Mesh{};
    std::unique_ptr<Shader> m_Shader{};
};


#pragma once

#include "RockHopper/Event/Layer.hh"
#include "RockHopper/Engine/EngineEvents.hh"
#include "RockHopper/Window/WindowEvents.hh"
#include "RockHopper/Rendering/Shader.hh"
#include "RockHopper/Rendering/Mesh.hh"
#include "RockHopper/Rendering/Transform.hh"
using namespace RockHopper;

class Cube
    : public Layer<EngineEvent,WindowEvent>
{
public:
    virtual ~Cube() = default;
    explicit Cube();

private:
    virtual void on_event(WindowInitEvent const&) override;
    virtual void on_event(WindowDisposeEvent const&) override;

    virtual void on_event(EngineTickEvent const&) override;
    virtual void on_event(WindowRefreshEvent const&) override;

private:
    std::unique_ptr<Shader> m_Shader{};
    std::unique_ptr<Mesh> m_Mesh{};
    std::unique_ptr<Transform> m_Transform{};

    TimeSpan m_SigmaTime{};
};

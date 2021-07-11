
#pragma once

#include "RockHopper/Layer/Layer.hh"
#include "RockHopper/Window/WindowEvents.hh"
#include "RockHopper/Window/Rendering/Shader.hh"

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
    unsigned int m_VertexArray{};
    unsigned int m_VertexBuffer{};
    unsigned int m_IndexBuffer{};

    Shader m_Shader{};
};

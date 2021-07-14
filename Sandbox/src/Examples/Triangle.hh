
#pragma once

#include "RockHopper/Layer/Layer.hh"
#include "RockHopper/Window/WindowEvents.hh"
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
    unsigned int m_VertexArray{};
    unsigned int m_VertexBuffer{};
    unsigned int m_IndexBuffer{};

    std::unique_ptr<Shader> m_Shader = Shader::Create();
};


#include "Sandbox.hh"

#include "RockHopper/Debug.hh"
#include "RockHopper/Window/Platform/GlfwWindow.hh"
#include "RockHopper/Rendering/Renderer.hh"

#include <GLFW/glfw3.h>

static RockHopper::WindowDetails GetInitialWindowDetails()
{
    return RockHopper::WindowDetails
    {
        .title = "RockHopper Client",
        .width = 800, .height = 600,
        .frametime = 1'000'000'000 / 60,
    };
}

Sandbox::Sandbox()
    : m_Engine{std::make_unique<Engine>("default")}
    , m_Window{std::make_unique<GlfwWindow>(Renderer::API::OpenGL,GetInitialWindowDetails())}
    , m_Keyboard{std::make_unique<Keyboard>("default")}
    , m_Mouse{std::make_unique<Mouse>("default")}
{
    m_Window->attach(m_Keyboard.get());
    m_Window->attach(m_Mouse.get());

    m_Window->insert_event_listener(&m_Background);
    m_Keyboard->insert_event_listener(&m_Background);
    m_Mouse->insert_event_listener(&m_Background);

    m_Window->insert_event_listener(&m_Triangle);
}

Sandbox::~Sandbox()
{
    m_Window->insert_event_listener(&m_Triangle);

    m_Window->remove_event_listener(&m_Background);
    m_Keyboard->remove_event_listener(&m_Background);
    m_Mouse->remove_event_listener(&m_Background);

    m_Window->detach(m_Keyboard.get());
    m_Window->detach(m_Mouse.get());
}

void Sandbox::run()
{
    WaitVariable stop = m_Window->start();
    stop.wait();
}

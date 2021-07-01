
#include "Sandbox.hh"

#include "RockHopper/Logging/Logger.hh"
#include "RockHopper/Event/EventListeners.hh"
#include "RockHopper/Input/Keyboard/KeyEvents.hh"
#include "RockHopper/Input/Mouse/MouseEvents.hh"

#include <GLFW/glfw3.h>

static RockHopper::WindowDetails GetInitialWindowDetails()
{
    return RockHopper::WindowDetails
    {
        .width = 800,
        .height = 600,
        .title = "RockHopper Client",
        .frametime = 1'000'000'000 / 60,
    };
}

Sandbox::Sandbox()
    : m_Window{GetInitialWindowDetails()}
    , m_Keyboard{}
    , m_Mouse{}
{
    using namespace RockHopper;

    // Configure and attach the Keyboard
    m_Keyboard.give_event_listener(EventFunctionListener<KeyPressEvent>{[](KeyPressEvent const& event)
    {
        if (event.key == KeyCode::KEY_SPACE)
        {
            float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            glClearColor(r,g,b,1);
        }
    }});
    m_Keyboard.give_event_listener(EventFunctionListener<KeyPressEvent>{[](KeyPressEvent const& event)
    {
        if (event.key == KeyCode::KEY_Q)
        {
            ROCKHOPPER_LOG_DEBUG("Querying the state of KEY_W - {}.",(event.keyboard->key(KeyCode::KEY_W).down() ? "down" : "up"));
        }
    }});
    m_Window.attach(&m_Keyboard);

    // Configure and attach the Mouse
    m_Mouse.give_event_listener(EventFunctionListener<MouseDragEvent>{[](MouseDragEvent const& event)
    {
        WindowDetails details = event.mouse->window()->get_details();
        float r = event.x / details.width;
        float g = 0.5f;
        float b = event.y / details.height;
        glClearColor(r,g,b,1);
    }});
    m_Mouse.give_event_listener(EventFunctionListener<MousePressEvent>{[](MousePressEvent const& event)
    {
        if (event.button == MouseCode::BUTTON_RIGHT)
        {
            ROCKHOPPER_LOG_DEBUG("Querying the state of BUTTON_LEFT - {}.",(event.mouse->key(MouseCode::BUTTON_LEFT).down() ? "down" : "up"));
        }
    }});
    m_Window.attach(&m_Mouse);

    // Configure and attach an input layer for extra input events.
    m_InputLayer.handler<KeyEvent>(1).give_event_listener(EventFunctionListener<KeyPressEvent>{[](KeyPressEvent const& event)
    {
        if (event.key == KeyCode::KEY_S)
        {
            ROCKHOPPER_LOG_INFO("Detected KEY_S pressed on layer 1!");
        }
    }});
    m_InputLayer.handler<KeyEvent>(2).give_event_listener(EventFunctionListener<KeyPressEvent>{[](KeyPressEvent const& event)
    {
        if (event.key == KeyCode::KEY_S)
        {
            ROCKHOPPER_LOG_INFO("Detected KEY_S pressed on layer 2!");
        }
    }});
    m_InputLayer.handler<MouseEvent>(2).give_event_listener(EventFunctionListener<MousePressEvent>{[](MousePressEvent const& event)
    {
        ROCKHOPPER_LOG_INFO("Detected a MOUSE_PRESS event on layer 2");
    }});
    m_Keyboard.insert_event_listener(&m_InputLayer);
    m_Mouse.insert_event_listener(&m_InputLayer);
}

Sandbox::~Sandbox()
{
}

void Sandbox::run()
{
    using namespace RockHopper;

    EventWaitListener<EngineDisposeEvent> eol_listener;
    m_Window.Engine::insert_event_listener(&eol_listener);

    m_Window.start();
    eol_listener.wait();

    // m_Window will destruct here
}

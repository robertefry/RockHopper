
#include "Background.hh"

#include "RockHopper/Window/Window.hh"

#include <GLFW/glfw3.h>

Background::~Background()
{
}

Background::Background()
    : m_ClearColor{0.1f,0.1f,0.1f,1.0f}
{
}

void Background::on_event(WindowRefreshEvent const& event)
{
    std::lock_guard lock {m_Mutex};
    glClearColor(m_ClearColor[0],m_ClearColor[1],m_ClearColor[2],m_ClearColor[3]);
}

void Background::on_event(KeyPressEvent const& event)
{
    if (event.key == KeyCode::KEY_SPACE)
    {
        float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        float g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        float b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

        std::lock_guard lock {m_Mutex};
        m_ClearColor = {r,g,b,1};
    }
    if (event.key == KeyCode::KEY_Q)
    {
        std::string state = event.keyboard->key(KeyCode::KEY_W).down() ? "down" : "up";
        ROCKHOPPER_LOG_DEBUG("Querying the state of KEY_W - {}.",state);
    }
    if (event.key == KeyCode::KEY_R)
    {
        m_ClearColor = {0.1f,0.1f,0.1f,1.0f};
    }
}

void Background::on_event(MousePressEvent const& event)
{
    if (event.button == MouseCode::BUTTON_RIGHT)
    {
        std::string state = event.mouse->key(MouseCode::BUTTON_LEFT).down() ? "down" : "up";
        ROCKHOPPER_LOG_DEBUG("Querying the state of BUTTON_LEFT - {}.",state);
    }
}

void Background::on_event(MouseDragEvent const& event)
{
    WindowDetails details = event.mouse->window()->get_details();
    float r = event.x / details.width;
    float g = 0.5f;
    float b = event.y / details.height;

    std::lock_guard lock {m_Mutex};
    m_ClearColor = {r,g,b,1};
}

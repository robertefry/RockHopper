
#include "RockHopper/UI/Window/GLFW.hh"
using namespace RockHopper;

#include "RockHopper/Event/EventListener.hh"
#include "RockHopper/Debug.hh"

#include <thread-tools/wait_lock.hh>
#include <gtest/gtest.h>

TEST(UI,WINDOW_GLFW)
{
    // using TickThread = Chrono::TickThread<Event::Dispatch::SequentialDispatch>;
    // auto tick_thread = std::make_shared<TickThread>();
    // auto renderer = std::make_shared<GX::Renderer>();

    // auto window = std::make_shared<UI::GLFW::Window>(tick_thread,renderer);
    // window->set_width(1600); window->set_height(800);

    // tick_thread->start();
    // window->show();

    // thd::wait_lock wait_lock;
    // window->insert_event_listener(
    //     Event::MakeListener<UI::WindowEvent_Destroy>([&](auto event)
    // {
    //     ROCKHOPPER_LOG_DEBUG("processing disposal event");
    //     wait_lock.notify_all();
    // }));
    // wait_lock.wait(); // TODO: FINISH UI.WINDOW_GLFW TESTS
}

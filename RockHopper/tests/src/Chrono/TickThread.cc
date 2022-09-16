
#include "RockHopper/Chrono/TickThread.hh"
using namespace RockHopper::Chrono;

#include "RockHopper/Event/EventListener.hh"
using namespace RockHopper::Event;

#include "RockHopper/Logging/Logger.hh"

#include <thread-tools/wait_lock.hh>
#include <gtest/gtest.h>

TEST(CHRONO,TICK_THREAD)
{
    struct TestTickThread
        : public TickThread<Dispatch::SequentialDispatch>
    {
        virtual void on_event(TickEvent_OnTick const&) override
        {
            ROCKHOPPER_LOG_INFO("TestTickThread::on_event(TickEvent_OnTick const&)");
            thread_id = std::this_thread::get_id();
            stop(); // stop the thread after the first tick
        }
        std::thread::id thread_id {0};
    };

    TestTickThread tick_thread;

    thd::wait_lock wait_lock;
    tick_thread.insert_event_listener(MakeListener<TickEvent_Dispose>([&](TickEvent_Dispose const&)
    {
        wait_lock.notify_all();
    }));

    tick_thread.start();

    wait_lock.wait(); // wait for the tick_thread to stop
    // expect tick_thread::on_event(TickEvent_OnTick const&) called on a different thread
    EXPECT_NE(tick_thread.thread_id,std::thread::id{0});
    EXPECT_NE(tick_thread.thread_id,std::this_thread::get_id());
}


#include <catch2/catch_all.hpp>

#include "RockHopper/Chrono/TickThread.hh"
#include "RockHopper/Event/EventDispatch.Sequential.hh"

TEST_CASE("Chrono::TickThread calls its own event listener")
{
    using Dispatch = RockHopper::Event::Dispatch::Sequential;
    using TickThread = RockHopper::Chrono::TickThread<Dispatch>;

    struct TestThread : TickThread
    {
        using TickEvent_Init = RockHopper::Chrono::TickEvent_Init;
        void on_event(TickEvent_Init const&) override
        {
            count_init += 1;
        }
        std::atomic<size_t> count_init = 0;

        using TickEvent_Dispose = RockHopper::Chrono::TickEvent_Dispose;
        void on_event(TickEvent_Dispose const&) override
        {
            count_dispose += 1;
        }
        std::atomic<size_t> count_dispose = 0;

        using TickEvent_OnTick = RockHopper::Chrono::TickEvent_OnTick;
        void on_event(TickEvent_OnTick const&) override
        {
            count_tick += 1;
            thread_id = std::this_thread::get_id();
            stop(); // stop this thread after the first tick
        }
        std::atomic<size_t> count_tick = 0;

        std::thread::id thread_id = std::this_thread::get_id();
    };
    TestThread tick_thread;

    REQUIRE(tick_thread.count_init == 0);
    REQUIRE(tick_thread.count_dispose == 0);
    REQUIRE(tick_thread.count_tick == 0);
    REQUIRE(tick_thread.thread_id == std::this_thread::get_id());

    tick_thread.start();
    // tick_thread should automatically stop after the first tick

    // wait for tick_thread to stop
    while (tick_thread.count_dispose == 0) { /* spin */ }

    REQUIRE(tick_thread.count_init == 1);
    REQUIRE(tick_thread.count_dispose == 1);
    REQUIRE(tick_thread.count_tick == 1);
    REQUIRE(tick_thread.thread_id != std::this_thread::get_id());
}

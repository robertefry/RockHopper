
#include "RockHopper/Utility/Async/WaitLock.hh"

#include <gtest/gtest.h>
#include <thread>

TEST(UTILITY_ASYNC__WAIT_LOCK,WAIT_BEFORE_NOTIFY)
{
    RockHopper::WaitLock lock;

    std::function const f_consumer =
        [](RockHopper::WaitLock::Async async)
    {
        EXPECT_TRUE(async.alive());

        bool notified = false;
        while (not notified)
        {
            notified = async.wait();
        }
        EXPECT_TRUE(notified);
    };
    std::thread t_consumer {f_consumer,lock.async()};

    while (lock.num_waiting() == 0)
    {
        // spin while waiting for f_consumer to wait
    }
    lock.notify_all();

    t_consumer.join();
}

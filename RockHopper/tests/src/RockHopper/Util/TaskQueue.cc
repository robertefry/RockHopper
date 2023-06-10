
#include "RockHopper/Util/TaskQueue.hh"
using namespace RockHopper::Util;

#include <catch2/catch_all.hpp>

TEST_CASE("TaskQueue: destruction does not invalidate producers or consumers")
{
    TaskQueue* task_queue = new TaskQueue{};
    TaskQueue::Producer producer = task_queue->make_producer();
    TaskQueue::Consumer consumer = task_queue->make_consumer();
    delete task_queue;

    int count = 0;

    producer.push_task([&](){ count += 1; });
    REQUIRE(count == 0);

    consumer.try_execute_one();
    REQUIRE(count == 1);
}

TEST_CASE("TaskQueue: SPSC: tasks are executed sequentially per-producer")
{
    TaskQueue task_queue;
    TaskQueue::Producer producer = task_queue.make_producer();
    TaskQueue::Consumer consumer = task_queue.make_consumer();

    static constexpr size_t idx_max = 256;
    size_t last_idx = 0;

    for (size_t idx = 1; idx < idx_max; ++idx)
    {
        producer.push_task([&,idx](){ last_idx = idx; });
    }

    for (size_t idx = 1; idx < idx_max; ++idx)
    {
        consumer.try_execute_one();
        REQUIRE(last_idx == idx);
    }
}

TEST_CASE("TaskQueue: MPSC: tasks are executed sequentially per-producer")
{
    TaskQueue task_queue;
    TaskQueue::Consumer consumer = task_queue.make_consumer();

    struct Producer
    {
        TaskQueue::Producer m_Producer;
        std::vector<size_t> m_Sequence {};

        /* static constexpr */ size_t const s_MaxSequence = 256;

        explicit Producer(TaskQueue& task_queue)
            : m_Producer {task_queue.make_producer()}
        {
            for (size_t idx = 0; idx < s_MaxSequence; ++idx)
            {
                auto task = [&,idx]()
                {
                    m_Sequence.push_back(idx);
                };
                m_Producer.push_task(std::move(task));
            }
        }

        void require_correct() const
        {
            for (size_t idx = 0; idx < s_MaxSequence; ++idx)
            {
                REQUIRE(m_Sequence[idx] == idx);
            }
        }
    };

    static constexpr size_t s_NumProducers = 16;
    std::vector<Producer> producers; producers.reserve(s_NumProducers);

    for (size_t i = 0; i < s_NumProducers; ++i)
    {
        producers.emplace_back(task_queue);
    }

    bool success = false;
    do
    {
        success = consumer.try_execute_one();
    }
    while (success);

    for (auto& producer : producers)
    {
        producer.require_correct();
    }
}


#include "RockHopper/Util/TaskQueue.hh"

namespace RockHopper::Util
{

    TaskQueue::~TaskQueue()
    {
        while (true)
        {
            bool const success = try_execute_one();
            if (not success) break;
        }
    }

    TaskQueue::TaskQueue()
        : m_ExecutorQueue{std::make_unique<ExecutorQueue>()}
    {}

    TaskQueue::TaskQueue(size_t capacity)
        : m_ExecutorQueue{std::make_unique<ExecutorQueue>(capacity)}
    {}

    auto TaskQueue::size() const -> size_t
    {
        return m_ExecutorQueue->size_approx();
    }

    TaskQueue::Producer::~Producer()
    {
        while (true)
        {
            std::unique_ptr<I_Executor> executor;
            bool const success = m_ExecutorQueue->try_dequeue_from_producer(m_ProducerToken,executor);
            if (not success) break;

            // drop the unprocessed task
            ROCKHOPPER_LOG_ERROR("Unprocessed task dropped from a TaskQueue.");
        }
    }

    TaskQueue::Producer::Producer(std::shared_ptr<ExecutorQueue> const& executor_queue)
        : m_ExecutorQueue{executor_queue}
        , m_ProducerToken{*executor_queue}
    {}

    auto TaskQueue::make_producer() -> TaskQueue::Producer
    {
        return Producer{m_ExecutorQueue};
    }

    TaskQueue::Consumer::~Consumer() = default;

    TaskQueue::Consumer::Consumer(std::shared_ptr<ExecutorQueue> const& executor_queue)
        : m_ExecutorQueue{executor_queue}
        , m_ConsumerToken{*executor_queue}
    {}

    auto TaskQueue::Consumer::size() const -> size_t
    {
        return m_ExecutorQueue->size_approx();
    }

    bool TaskQueue::Consumer::try_execute_one()
    {
        std::unique_ptr<I_Executor> executor;
        bool const success = m_ExecutorQueue->try_dequeue(m_ConsumerToken,executor);

        if (success)
        {
            executor->operator()();
        }
        return success;
    }

    auto TaskQueue::make_consumer() -> TaskQueue::Consumer
    {
        return Consumer{m_ExecutorQueue};
    }

    bool TaskQueue::try_execute_one()
    {
        std::unique_ptr<I_Executor> executor;
        bool const success = m_ExecutorQueue->try_dequeue(executor);

        if (success)
        {
            executor->operator()();
        }
        return success;
    }

} // namespace RockHopper::Util

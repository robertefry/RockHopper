
#include "RockHopper/Utility/TaskQueue.hh"

namespace RockHopper
{

    size_t TaskQueue::size() const
    {
        std::lock_guard<std::mutex> lock {m_TaskQueueMutex};
        return m_TaskQueue.size();
    }

    void TaskQueue::give(TaskFunc const& task)
    {
        std::lock_guard<std::mutex> lock {m_TaskQueueMutex};
        m_TaskQueue.push(task);
    }

    void TaskQueue::give(TaskFunc&& task)
    {
        std::lock_guard<std::mutex> lock {m_TaskQueueMutex};
        m_TaskQueue.push(std::move(task));
    }

    void TaskQueue::execute_one()
    {
        std::lock_guard<std::mutex> lock {m_TaskQueueMutex};
        m_TaskQueue.front()();
        m_TaskQueue.pop();
    }

    void TaskQueue::execute_all()
    {
        std::lock_guard<std::mutex> lock {m_TaskQueueMutex};

        while (m_TaskQueue.size())
        {
            m_TaskQueue.front()();
            m_TaskQueue.pop();
        }
    }

    void TaskQueue::clear()
    {
        std::lock_guard<std::mutex> lock {m_TaskQueueMutex};

        while (m_TaskQueue.size())
        {
            m_TaskQueue.pop();
        }
    }

} // namespace RockHopper

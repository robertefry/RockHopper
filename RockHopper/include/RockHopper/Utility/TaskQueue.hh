
#ifndef __HH_ROCKHOPPER_UTIL_TASK_QUEUE_
#define __HH_ROCKHOPPER_UTIL_TASK_QUEUE_

#include <queue>
#include <mutex>

namespace RockHopper
{

    class TaskQueue
    {
    public:
        virtual ~TaskQueue() = default;

        typedef void(*TaskFunc)(void);

        void give(TaskFunc const&);
        void give(TaskFunc&&);

        size_t size() const;
        void execute_one();
        void execute_all();
        void clear();

    private:
        std::queue<TaskFunc> m_TaskQueue{};
        mutable std::mutex m_TaskQueueMutex{};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_UTIL_TASK_QUEUE_ */

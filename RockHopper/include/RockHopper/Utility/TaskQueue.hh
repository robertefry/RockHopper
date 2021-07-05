
#ifndef __HH_ROCKHOPPER_UTIL_TASK_QUEUE_
#define __HH_ROCKHOPPER_UTIL_TASK_QUEUE_

#include "TaskQueue.fwd"

#include <functional>
#include <queue>
#include <future>
#include <condition_variable>
#include <mutex>

/* ************************************************************************** */
// [Definition] RockHopper::TaskQueue
/* ************************************************************************** */

namespace RockHopper
{

    template <typename T_Res, typename... T_Args>
    class TaskQueue<T_Res(T_Args...)>
    {
        class FutureExecutor
        {
        public:
            using ReturnType = std::future<T_Res>;
            using TaskFunc = std::function<T_Res(T_Args...)>;

            explicit FutureExecutor(TaskFunc const& task);
            explicit FutureExecutor(TaskFunc&& task);

            void operator()(T_Args... args);
            ReturnType future();

        private:
            TaskFunc m_Function;
            std::promise<T_Res> m_Promise{};
        };

    public:
        using ReturnType = FutureExecutor::ReturnType;
        using TaskFunc = FutureExecutor::TaskFunc;

        virtual ~TaskQueue() = default;
        explicit TaskQueue() = default;

        explicit TaskQueue(TaskQueue&&);

        void wait();
        template <typename T_Rep, typename T_Period>
        std::cv_status wait_for(std::chrono::duration<T_Rep,T_Period> const& rtime);
        template <typename T_Clock, typename T_Duration>
        std::cv_status wait_until(std::chrono::time_point<T_Clock,T_Duration> const& atime);

        ReturnType push_task(TaskFunc const& task);
        ReturnType push_task(TaskFunc&& task);

        inline void wait_task(TaskFunc const& task) { push_task(task).wait(); }
        inline void wait_task(TaskFunc&& task) { push_task(std::move(task)).wait(); }

        size_t size() const;
        void execute_one(T_Args...);
        void execute_all(T_Args...);

    private:
        std::queue<FutureExecutor> m_TaskQueue{};
        std::condition_variable m_TaskQueueNotifier{};
        std::mutex m_TaskQueueMutex{};
    };

} // namespace RockHopper

/* ************************************************************************** */
// [Implementation] RockHopper::TaskQueue
/* ************************************************************************** */

namespace RockHopper
{

    template <typename T_Res, typename... T_Args>
    TaskQueue<T_Res(T_Args...)>::FutureExecutor::FutureExecutor(TaskFunc const& task)
        : m_Function{task}
    {
    }

    template <typename T_Res, typename... T_Args>
    TaskQueue<T_Res(T_Args...)>::FutureExecutor::FutureExecutor(TaskFunc&& task)
        : m_Function{std::move(task)}
    {
    }

    template <typename T_Res, typename... T_Args>
    void TaskQueue<T_Res(T_Args...)>::FutureExecutor::operator()(T_Args... args)
    {
        if constexpr (std::is_same<T_Res,void>::value)
        {
            m_Function(args...);
            m_Promise.set_value();
        }
        else
        {
            T_Res result = m_Function(args...);
            m_Promise.set_value(std::move(result));
        }
    }

    template <typename T_Res, typename... T_Args>
    auto TaskQueue<T_Res(T_Args...)>::FutureExecutor::future()
        -> TaskQueue<T_Res(T_Args...)>::FutureExecutor::ReturnType
    {
        return m_Promise.get_future();
    }

    template <typename T_Res, typename... T_Args>
    TaskQueue<T_Res(T_Args...)>::TaskQueue(TaskQueue&& other)
    {
        std::lock_guard<std::mutex> lock_1 {m_TaskQueueMutex,std::defer_lock};
        std::lock_guard<std::mutex> lock_2 {other.m_TaskQueueMutex,std::defer_lock};
        std::lock(lock_1,lock_2);

        m_TaskQueue = std::move(other.m_TaskQueue);
    }

    template <typename T_Res, typename... T_Args>
    void TaskQueue<T_Res(T_Args...)>::wait()
    {
        std::unique_lock<std::mutex> lock {m_TaskQueueMutex};
        m_TaskQueueNotifier.wait(lock);
    }

    template <typename T_Res, typename... T_Args>
    template <typename T_Rep, typename T_Period>
    std::cv_status TaskQueue<T_Res(T_Args...)>::wait_for(std::chrono::duration<T_Rep,T_Period> const& rtime)
    {
        std::unique_lock<std::mutex> lock {m_TaskQueueMutex};
        return m_TaskQueueNotifier.wait_for(lock,rtime);
    }

    template <typename T_Res, typename... T_Args>
    template <typename T_Clock, typename T_Duration>
    std::cv_status TaskQueue<T_Res(T_Args...)>::wait_until(std::chrono::time_point<T_Clock,T_Duration> const& atime)
    {
        std::unique_lock<std::mutex> lock {m_TaskQueueMutex};
        return m_TaskQueueNotifier.wait_until(lock,atime);
    }

    template <typename T_Res, typename... T_Args>
    auto TaskQueue<T_Res(T_Args...)>::push_task(TaskFunc const& task)
        -> TaskQueue<T_Res(T_Args...)>::ReturnType
    {
        std::lock_guard<std::mutex> lock {m_TaskQueueMutex};
        m_TaskQueue.emplace(task);
        m_TaskQueueNotifier.notify_all();
        return m_TaskQueue.back().future();
    }

    template <typename T_Res, typename... T_Args>
    auto TaskQueue<T_Res(T_Args...)>::push_task(TaskFunc&& task)
        -> TaskQueue<T_Res(T_Args...)>::ReturnType
    {
        std::lock_guard<std::mutex> lock {m_TaskQueueMutex};
        m_TaskQueue.emplace(std::move(task));
        m_TaskQueueNotifier.notify_all();
        return m_TaskQueue.back().future();
    }

    template <typename T_Res, typename... T_Args>
    size_t TaskQueue<T_Res(T_Args...)>::size() const
    {
        std::lock_guard<std::mutex> lock {m_TaskQueueMutex};
        return m_TaskQueue.size();
    }

    template <typename T_Res, typename... T_Args>
    void TaskQueue<T_Res(T_Args...)>::execute_one(T_Args... args)
    {
        m_TaskQueue.front()(args...);

        std::lock_guard<std::mutex> lock {m_TaskQueueMutex};
        m_TaskQueue.pop();
    }

    template <typename T_Res, typename... T_Args>
    void TaskQueue<T_Res(T_Args...)>::execute_all(T_Args... args)
    {
        while (m_TaskQueue.size())
        {
            execute_one();
        }
    }

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_UTIL_TASK_QUEUE_ */

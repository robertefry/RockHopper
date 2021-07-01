
#ifndef __HH_ROCKHOPPER_UTIL_TASK_QUEUE_
#define __HH_ROCKHOPPER_UTIL_TASK_QUEUE_

#include "TaskQueue.fwd"

#include <functional>
#include <future>
#include <queue>
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
            using TaskFunc = std::function<T_Res(T_Args...)>;

            explicit FutureExecutor(TaskFunc const& func);
            explicit FutureExecutor(TaskFunc&& func);

            void operator()(T_Args... args);
            std::future<T_Res> future();

        private:
            TaskFunc m_Function;
            std::promise<T_Res> m_Promise{};
        };

    public:
        using TaskFunc = FutureExecutor::TaskFunc;
        virtual ~TaskQueue() = default;
        explicit TaskQueue() = default;

        std::future<T_Res> push(TaskFunc const& func);
        std::future<T_Res> push(TaskFunc&& func);

        size_t size() const;
        void execute_one(T_Args...);
        void execute_all(T_Args...);

    private:
        std::queue<FutureExecutor> m_TaskQueue{};
        std::mutex m_TaskQueueMutex{};
    };

} // namespace RockHopper

/* ************************************************************************** */
// [Implementation] RockHopper::TaskQueue
/* ************************************************************************** */

namespace RockHopper
{

    template <typename T_Res, typename... T_Args>
    TaskQueue<T_Res(T_Args...)>::FutureExecutor::FutureExecutor(TaskFunc const& func)
        : m_Function{func}
    {
    }

    template <typename T_Res, typename... T_Args>
    TaskQueue<T_Res(T_Args...)>::FutureExecutor::FutureExecutor(TaskFunc&& func)
        : m_Function{std::move(func)}
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
    std::future<T_Res> TaskQueue<T_Res(T_Args...)>::FutureExecutor::future()
    {
        return m_Promise.get_future();
    }

    template <typename T_Res, typename... T_Args>
    std::future<T_Res> TaskQueue<T_Res(T_Args...)>::push(TaskFunc const& func)
    {
        std::lock_guard<std::mutex> lock {m_TaskQueueMutex};
        m_TaskQueue.emplace(func);
        return m_TaskQueue.back().future();
    }

    template <typename T_Res, typename... T_Args>
    std::future<T_Res> TaskQueue<T_Res(T_Args...)>::push(TaskFunc&& func)
    {
        std::lock_guard<std::mutex> lock {m_TaskQueueMutex};
        m_TaskQueue.emplace(std::move(func));
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
        std::lock_guard<std::mutex> lock {m_TaskQueueMutex};
        m_TaskQueue.front()(args...);
        m_TaskQueue.pop();
    }

    template <typename T_Res, typename... T_Args>
    void TaskQueue<T_Res(T_Args...)>::execute_all(T_Args... args)
    {
        std::lock_guard<std::mutex> lock {m_TaskQueueMutex};

        while (m_TaskQueue.size())
        {
            m_TaskQueue.front()(args...);
            m_TaskQueue.pop();
        }
    }

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_UTIL_TASK_QUEUE_ */

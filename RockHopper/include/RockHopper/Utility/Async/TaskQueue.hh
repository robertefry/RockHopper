
#ifndef __HH_ROCKHOPPER_UTIL_ASYNC_TASK_QUEUE_
#define __HH_ROCKHOPPER_UTIL_ASYNC_TASK_QUEUE_

#include "TaskQueue.fwd"

#include "RockHopper/Utility/Async/WaitLock.hh"

#include <functional>
#include <tuple>
#include <queue>
#include <memory>
#include <future>
#include <condition_variable>
#include <mutex>

/* ************************************************************************** */
// [Definition] RockHopper::TaskQueue
/* ************************************************************************** */

namespace RockHopper
{

    class TaskQueue
    {
        struct I_Executor
        {
            virtual ~I_Executor() = default;
            virtual void operator()() = 0;
        };

        template <typename T_Signature>
        class Executor;

        template <typename T_Ret, typename... T_Args>
        class Executor<T_Ret(T_Args...)> : public I_Executor
        {
        public:
            explicit Executor(std::function<T_Ret(T_Args...)> const& task, T_Args&&... args)
                : m_PackagedTask{task}, m_PackagedArgs{std::forward<T_Args>(args)...}
            {}
            explicit Executor(std::function<T_Ret(T_Args...)>&& task, T_Args&&... args)
                : m_PackagedTask{std::move(task)}, m_PackagedArgs{std::forward<T_Args>(args)...}
            {}
            std::future<T_Ret> get_future()
            {
                return m_PackagedTask.get_future();
            }
            virtual void operator()() override
            {
                std::apply(m_PackagedTask,m_PackagedArgs);
            }
        private:
            std::packaged_task<T_Ret(T_Args...)> m_PackagedTask;
            std::tuple<T_Args...> m_PackagedArgs;
        };

    public:
        virtual ~TaskQueue() = default;
        explicit TaskQueue() = default;

        inline explicit TaskQueue(TaskQueue&&);

        template <typename T_Func, typename... T_Args>
        auto push_task(T_Func&& func, T_Args&&... args)
            -> std::future<typename std::invoke_result<T_Func,T_Args...>::type>;
        template <typename T_Func, typename... T_Args>
        auto wait_task(T_Func&& func, T_Args&&... args)
            -> std::invoke_result<T_Func,T_Args...>::type;

        inline auto insert_notifier() const { return m_InsertLock.async(); }

        inline size_t size() const;
        inline void execute_one();
        inline void execute_all();

    private:
        std::queue<std::unique_ptr<I_Executor>> m_TaskQueue{};
        WaitLock m_InsertLock{};
        mutable std::mutex m_TaskQueueMutex{};
    };

} // namespace RockHopper

/* ************************************************************************** */
// [Implementation] RockHopper::TaskQueue
/* ************************************************************************** */

namespace RockHopper
{

    TaskQueue::TaskQueue(TaskQueue&& other)
    {
        std::scoped_lock<std::mutex,std::mutex> lock {m_TaskQueueMutex,other.m_TaskQueueMutex};
        m_TaskQueue = std::move(other.m_TaskQueue);
    }

    template <typename T_Func, typename... T_Args>
    auto TaskQueue::push_task(T_Func&& func, T_Args&&... args)
        -> std::future<typename std::invoke_result<T_Func,T_Args...>::type>
    {
        using T_Ret = std::invoke_result<T_Func,T_Args...>::type;
        using T_Executor = Executor<T_Ret(T_Args...)>;

        std::unique_ptr<T_Executor> executor = std::make_unique<T_Executor>(
            std::forward<T_Func>(func), std::forward<T_Args>(args)...
        );
        std::future<T_Ret> future = executor->get_future();

        std::lock_guard<std::mutex> lock {m_TaskQueueMutex};
        m_TaskQueue.push(std::move(executor));
        m_InsertLock.notify_one();
        return future;
    }

    template <typename T_Func, typename... T_Args>
    auto TaskQueue::wait_task(T_Func&& func, T_Args&&... args)
        -> std::invoke_result<T_Func,T_Args...>::type
    {
        using T_Ret = std::invoke_result<T_Func,T_Args...>::type;

        std::future<T_Ret> future = push_task(
            std::forward<T_Func>(func), std::forward<T_Args>(args)...
        );
        future.wait();
        return future.get();
    }

    size_t TaskQueue::size() const
    {
        std::lock_guard<std::mutex> lock {m_TaskQueueMutex};
        return m_TaskQueue.size();
    }

    void TaskQueue::execute_one()
    {
        m_TaskQueue.front()->operator()();

        std::lock_guard<std::mutex> lock {m_TaskQueueMutex};
        m_TaskQueue.pop();
    }

    void TaskQueue::execute_all()
    {
        while (size())
        {
            execute_one();
        }
    }

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_UTIL_ASYNC_TASK_QUEUE_ */

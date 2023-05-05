
#ifndef ROCKHOPPER_UTIL_TASK_QUEUE_HH
#define ROCKHOPPER_UTIL_TASK_QUEUE_HH

#include "RockHopper/Util/Lifetime.hh"

#include "concurrentqueue.h"

#include <memory>
#include <tuple>
#include <future>

namespace RockHopper::Util
{

    class TaskQueue
    {
        struct I_Executor : private Util::NoCopy
        {
            virtual ~I_Executor() = default;
            virtual void operator()() = 0;
        };

        template <typename T_Ret, typename... T_Args>
        class Executor : public I_Executor
        {
        public:
            template <typename T_Func>
            explicit Executor(T_Func&& func, T_Args&&... args)
                : m_PackagedTask{std::forward<T_Func>(func)}
                , m_PackagedArgs{std::forward<T_Args>(args)...}
            {}
            std::future<T_Ret> future()
            {
                return m_PackagedTask.get_future();
            }
            void operator()() override
            {
                std::apply(m_PackagedTask,m_PackagedArgs);
            }
        private:
            std::packaged_task<T_Ret(T_Args...)> m_PackagedTask;
            std::tuple<T_Args...> m_PackagedArgs;
        };

    public:
        virtual ~TaskQueue()
        {
            while (not empty()) execute_one();
        }

        explicit TaskQueue(size_t capacity)
            : m_ExecutorQueue{capacity}
        {}

        [[nodiscard]] inline auto size() const -> size_t { return m_ExecutorQueue.size_approx(); }
        [[nodiscard]] inline bool empty() const { return size() == 0; }

        template <typename T_Func, typename... T_Args>
        [[nodiscard]] auto push_task(T_Func&& func, T_Args&&... args)
        {
            using T_Ret = typename std::invoke_result<T_Func,T_Args...>::type;
            using T_Executor = Executor<T_Ret,T_Args...>;

            auto executor = std::make_unique<T_Executor>(
                std::forward<T_Func>(func), std::forward<T_Args>(args)...
            );
            auto future = executor->future();

            bool success = m_ExecutorQueue.enqueue(std::move(executor));
            if (not success) throw std::runtime_error{"failure to push to the task queue"};

            return future;
        }

        template <typename T_Func, typename... T_Args>
        [[nodiscard]] auto execute_task(T_Func&& func, T_Args&&... args)
        {
            using T_Ret = typename std::invoke_result<T_Func,T_Args...>::type;
            using T_Executor = Executor<T_Ret,T_Args...>;

            auto executor = T_Executor{
                std::forward<T_Func>(func), std::forward<T_Args>(args)...
            };
            executor.operator()();

            return executor.future();
        }

        bool execute_one()
        {
            std::unique_ptr<I_Executor> executor;

            bool success = m_ExecutorQueue.try_dequeue(executor);
            if (success) executor->operator()();

            return success;
        }

    private:
        using ExecutorQueue = moodycamel::ConcurrentQueue<std::unique_ptr<I_Executor>>;
        ExecutorQueue m_ExecutorQueue;
    };

} // namespace RockHopper::Util

#endif /* ROCKHOPPER_UTIL_TASK_QUEUE_HH */

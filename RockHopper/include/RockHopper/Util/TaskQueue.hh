
#ifndef ROCKHOPPER_UTIL_TASK_QUEUE_HH
#define ROCKHOPPER_UTIL_TASK_QUEUE_HH

#include "RockHopper/Logging/LoggerCore.enable.hh"

#include "RockHopper/Util/Lifetime.hh"

#include <concurrentqueue.h>

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
            [[nodiscard]] std::future<T_Ret> future()
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

    private:
        using ExecutorQueue = moodycamel::ConcurrentQueue<std::unique_ptr<I_Executor>>;
        ExecutorQueue m_ExecutorQueue;

    public:
        virtual ~TaskQueue()
        {
            while (true)
            {
                bool success = try_execute_one();
                if (not success) break;
            }
        }

        explicit TaskQueue()
        {}

        explicit TaskQueue(size_t capacity)
            : m_ExecutorQueue{capacity}
        {}

        [[nodiscard]] inline auto size() const -> size_t { return m_ExecutorQueue.size_approx(); }
        [[nodiscard]] inline bool empty() const { return size() == 0; }

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

    public:
        class Producer final
        {
            friend TaskQueue;

            explicit Producer(TaskQueue* task_queue)
                : m_TaskQueue{task_queue}
                , m_Token{task_queue->m_ExecutorQueue}
            {}

        public:
            virtual ~Producer()
            {
                while (true)
                {
                    std::unique_ptr<I_Executor> executor;
                    bool success = m_TaskQueue->m_ExecutorQueue.try_dequeue_from_producer(m_Token,executor);
                    if (not success) break;

                    // drop the unprocessed task
                    ROCKHOPPER_LOG_ERROR("Unprocessed task dropped from a TaskQueue.");
                }
            }

        public:
            template <typename T_Func, typename... T_Args>
            [[nodiscard]] auto push_task(T_Func&& func, T_Args&&... args)
            {
                using T_Ret = typename std::invoke_result<T_Func,T_Args...>::type;
                using T_Executor = Executor<T_Ret,T_Args...>;

                auto executor = std::make_unique<T_Executor>(
                    std::forward<T_Func>(func), std::forward<T_Args>(args)...
                );
                auto future = executor->future();

                bool success = m_TaskQueue->m_ExecutorQueue.enqueue(m_Token,std::move(executor));
                if (not success) throw std::runtime_error{"failure to push to the task queue"};

                return future;
            }

        private:
            TaskQueue* m_TaskQueue;
            moodycamel::ProducerToken m_Token;
        };

        [[nodiscard]] inline auto make_producer()
        {
            return Producer{this};
        }

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

    public:
        class Consumer final
        {
            friend TaskQueue;

            explicit Consumer(TaskQueue* task_queue)
                : m_TaskQueue{task_queue}
                , m_Token{task_queue->m_ExecutorQueue}
            {}

        public:
            [[nodiscard]] inline auto size() const -> size_t { return m_TaskQueue->size(); }
            [[nodiscard]] inline bool empty() const { return m_TaskQueue->empty(); }

            inline bool try_execute_one()
            {
                std::unique_ptr<I_Executor> executor;
                bool success = m_TaskQueue->m_ExecutorQueue.try_dequeue(m_Token,executor);

                if (success)
                {
                    executor->operator()();
                }
                return success;
            }

        private:
            TaskQueue* m_TaskQueue;
            moodycamel::ConsumerToken m_Token;
        };

        [[nodiscard]] inline auto make_consumer()
        {
            return Consumer{this};
        }

        inline bool try_execute_one()
        {
            std::unique_ptr<I_Executor> executor;
            bool success = m_ExecutorQueue.try_dequeue(executor);

            if (success)
            {
                executor->operator()();
            }
            return success;
        }
    };

} // namespace RockHopper::Util

#endif /* ROCKHOPPER_UTIL_TASK_QUEUE_HH */
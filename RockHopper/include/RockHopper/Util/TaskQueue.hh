
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
        std::shared_ptr<ExecutorQueue> m_ExecutorQueue;

    public:
        virtual ~TaskQueue();
        explicit TaskQueue();

        explicit TaskQueue(size_t capacity);

        [[nodiscard]] auto size() const -> size_t;
        [[nodiscard]] bool empty() const { return size() == 0; }

        template <typename T_Func, typename... T_Args>
        [[nodiscard]] auto execute_task(T_Func&& func, T_Args&&... args);

    public:
        class Producer
        {
            friend TaskQueue;

            std::shared_ptr<ExecutorQueue> m_ExecutorQueue;
            moodycamel::ProducerToken m_ProducerToken;

            explicit Producer(std::shared_ptr<ExecutorQueue> const& executor_queue);

        public:
            virtual ~Producer();

            Producer(Producer&&) = default;
            Producer& operator=(Producer&&) = default;

            template <typename T_Func, typename... T_Args>
            auto push_task(T_Func&& func, T_Args&&... args);
        };

        [[nodiscard]] Producer make_producer();

        template <typename T_Func, typename... T_Args>
        auto push_task(T_Func&& func, T_Args&&... args);

    public:
        class Consumer
        {
            friend TaskQueue;

            std::shared_ptr<ExecutorQueue> m_ExecutorQueue;
            moodycamel::ConsumerToken m_ConsumerToken;

            explicit Consumer(std::shared_ptr<ExecutorQueue> const& executor_queue);

        public:
            virtual ~Consumer();

            Consumer(Consumer&&) = default;
            Consumer& operator=(Consumer&&) = default;

            [[nodiscard]] auto size() const -> size_t;
            [[nodiscard]] bool empty() const { return size() == 0; }

            bool try_execute_one();
        };

        [[nodiscard]] Consumer make_consumer();

        bool try_execute_one();
    };

    template <typename T_Func, typename... T_Args>
    auto TaskQueue::execute_task(T_Func&& func, T_Args&&... args)
    {
        using T_Ret = typename std::invoke_result<T_Func,T_Args...>::type;
        using T_Executor = Executor<T_Ret,T_Args...>;

        auto executor = T_Executor{
            std::forward<T_Func>(func), std::forward<T_Args>(args)...
        };
        executor.operator()();

        return executor.future();
    }

    template <typename T_Func, typename... T_Args>
    auto TaskQueue::push_task(T_Func&& func, T_Args&&... args)
    {
        using T_Ret = typename std::invoke_result<T_Func,T_Args...>::type;
        using T_Executor = Executor<T_Ret,T_Args...>;

        auto executor = std::make_unique<T_Executor>(
            std::forward<T_Func>(func), std::forward<T_Args>(args)...
        );
        auto future = executor->future();

        bool success = m_ExecutorQueue->enqueue(std::move(executor));
        if (not success) throw std::runtime_error{"failure to push to the task queue"};

        return future;
    }

    template <typename T_Func, typename... T_Args>
    auto TaskQueue::Producer::push_task(T_Func&& func, T_Args&&... args)
    {
        using T_Ret = typename std::invoke_result<T_Func,T_Args...>::type;
        using T_Executor = Executor<T_Ret,T_Args...>;

        auto executor = std::make_unique<T_Executor>(
            std::forward<T_Func>(func), std::forward<T_Args>(args)...
        );
        auto future = executor->future();

        bool success = m_ExecutorQueue->enqueue(m_ProducerToken,std::move(executor));
        if (not success) throw std::runtime_error{"failure to push to the task queue"};

        return future;
    }

} // namespace RockHopper::Util

#endif /* ROCKHOPPER_UTIL_TASK_QUEUE_HH */

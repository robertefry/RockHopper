
#ifndef ROCKHOPPER_UTIL_TASK_QUEUE_HH
#define ROCKHOPPER_UTIL_TASK_QUEUE_HH

#include <boost/lockfree/queue.hpp>

#include <future>
#include <tuple>

namespace RockHopper::Util
{

    class TaskQueue
    {
        struct I_Executor
        {
            virtual ~I_Executor() = default;
            virtual void operator()() = 0;

            // default move disallows copy
            I_Executor(I_Executor&&) noexcept = default;
            I_Executor& operator=(I_Executor&&) noexcept = default;
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

        inline void reserve(size_t capacity) { m_ExecutorQueue.reserve(capacity); }

        [[nodiscard]] inline bool empty() const { return m_ExecutorQueue.empty(); }

        template <typename T_Func, typename... T_Args>
        [[nodiscard]] auto push_task(T_Func&& func, T_Args&&... args)
        {
            using T_Ret = typename std::invoke_result<T_Func,T_Args...>::type;
            using T_Executor = Executor<T_Ret,T_Args...>;

            auto executor = new T_Executor{
                std::forward<T_Func>(func), std::forward<T_Args>(args)...
            };
            auto future = executor->future();

            m_ExecutorQueue.push(executor);
            return future;
        }

        bool execute_one()
        {
            I_Executor* executor;
            bool success = m_ExecutorQueue.pop(executor);
            if (success) executor->operator()();

            return success;
        }

    private:
        using ExecutorQueue = boost::lockfree::queue<I_Executor*>;
        ExecutorQueue m_ExecutorQueue;
    };

} // namespace RockHopper::Util

#endif /* ROCKHOPPER_UTIL_TASK_QUEUE_HH */

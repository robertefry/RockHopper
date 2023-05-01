
#ifndef ROCKHOPPER_UTIL_THREAD_POOL_HH
#define ROCKHOPPER_UTIL_THREAD_POOL_HH

#include "RockHopper/Util/TaskQueue.hh"
#include "RockHopper/Util/Singleton.hh"

#include <vector>
#include <thread>
#include <atomic>

namespace RockHopper::Util
{

    class ThreadPool final
    {
        struct Impl : Util::TaskQueue
        {
            std::vector<std::thread> m_ThreadList{};
            std::atomic<bool> m_IsStopRequested = false;

            virtual ~Impl();
            explicit Impl();

            void run();
        };

    public:

        template <typename T_Func, typename... T_Args>
        [[nodiscard]] auto push_task(T_Func&& func, T_Args&&... args)
        {
            return m_Impl->push_task(
                std::forward<T_Func>(func), std::forward<T_Args>(args)...
            );
        }

    private:
        Util::Singleton<Impl> m_Impl{};
    };

} // namespace RockHopper::Util

#endif /* ROCKHOPPER_UTIL_THREAD_POOL_HH */

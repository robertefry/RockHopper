
#ifndef __HH_ROCKHOPPER_WAIT_VARIABLE_
#define __HH_ROCKHOPPER_WAIT_VARIABLE_

#include <mutex>
#include <condition_variable>

namespace RockHopper
{

    class WaitVariable
    {
    public:
        virtual ~WaitVariable() = default;
        explicit WaitVariable() = default;
    public:
        inline void wait() const noexcept
        {
            std::unique_lock<std::mutex> lock {m_Mutex};
            m_Condition.wait(lock);
        }
        template <typename T_Duration>
        inline void wait_for(T_Duration const& duration) const
        {
            std::unique_lock<std::mutex> lock {m_Mutex};
            m_Condition.wait_for(lock,duration);
        }
        template <typename T_TimePoint>
        inline void wait_until(T_TimePoint const& timepoint) const
        {
            std::unique_lock<std::mutex> lock {m_Mutex};
            m_Condition.wait_until(lock,timepoint);
        }
        inline void notify_one() noexcept
        {
            std::unique_lock<std::mutex> lock {m_Mutex};
            m_Condition.notify_one();
        }
        inline void notify_all() noexcept
        {
            std::unique_lock<std::mutex> lock {m_Mutex};
            m_Condition.notify_all();
        }
    private:
        mutable std::condition_variable m_Condition{};
        mutable std::mutex m_Mutex{};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WAIT_VARIABLE_ */

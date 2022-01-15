
#ifndef __HH_ROCKHOPPER_WAIT_VARIABLE_
#define __HH_ROCKHOPPER_WAIT_VARIABLE_

#include <memory>
#include <atomic>
#include <condition_variable>
#include <mutex>

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
            std::unique_lock<std::mutex> lock {m_DataPtr->m_Mutex};

            if (not m_DataPtr->m_Triggered)
            {
                m_DataPtr->m_Condition.wait(lock);
            }
        }
        template <typename T_Duration>
        inline void wait_for(T_Duration const& duration) const
        {
            std::unique_lock<std::mutex> lock {m_DataPtr->m_Mutex};

            if (not m_DataPtr->m_Triggered)
            {
                m_DataPtr->m_Condition.wait_for(lock,duration);
            }
        }
        template <typename T_TimePoint>
        inline void wait_until(T_TimePoint const& timepoint) const
        {
            std::unique_lock<std::mutex> lock {m_DataPtr->m_Mutex};

            if (not m_DataPtr->m_Triggered)
            {
                m_DataPtr->m_Condition.wait_until(lock,timepoint);
            }
        }
        inline void notify_all()
        {
            std::unique_lock<std::mutex> lock {m_DataPtr->m_Mutex};
            m_DataPtr->m_Triggered = true;
            m_DataPtr->m_Condition.notify_all();
        }
        inline void reset()
        {
            std::unique_lock<std::mutex> lock {m_DataPtr->m_Mutex};
            m_DataPtr->m_Triggered = false;
        }

    private:
        struct Data
        {
            std::atomic<bool> m_Triggered{};
            mutable std::condition_variable m_Condition{};
            mutable std::mutex m_Mutex{};
        };
        std::shared_ptr<Data> m_DataPtr = std::make_shared<Data>();

        friend bool operator==(WaitVariable const& var1, WaitVariable const& var2);
    };

    inline bool operator==(WaitVariable const& var1, WaitVariable const& var2)
    {
        return var1.m_DataPtr == var2.m_DataPtr;
    }

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WAIT_VARIABLE_ */

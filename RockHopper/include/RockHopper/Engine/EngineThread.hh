
#ifndef __HH_ROCKHOPPER_ENGINE_THREAD_
#define __HH_ROCKHOPPER_ENGINE_THREAD_

#include "RockHopper/Debug.hh"
#include "RockHopper/Utility/WaitVariable.hh"

#include <atomic>
#include <thread>

/* ************************************************************************** */
// [Implementation] RockHopper::EngineTiming
/* ************************************************************************** */

namespace RockHopper
{

    class EngineTiming
    {
    public:
        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = Clock::time_point;
        using TimeSpan = std::chrono::duration<int64_t,std::nano>;

        void init();
        void tick();
        void reduce();
        bool triggered() const;

        inline auto delta() const { return m_TimeDelta; }

        inline void set_omega(int64_t omega) { m_TimeOmega = TimeSpan{omega}; }
        inline auto get_omega() const -> TimeSpan const& { return m_TimeOmega; }

    private:
        TimePoint m_TimeLast{};
        TimeSpan m_TimeDelta{};
        TimeSpan m_TimeOmega{};
    };

} // namespace RockHopper

/* ************************************************************************** */
// [Implementation] RockHopper::EngineThread
/* ************************************************************************** */

namespace RockHopper
{

    class EngineThread
    {
    public:
        virtual ~EngineThread();
        explicit EngineThread(std::string const& name);

        virtual WaitVariable const& start();
        virtual WaitVariable const& stop();

        inline bool alive() const { return m_IsAlive; };

    private:
        void run();

    protected:
        virtual void init() = 0;
        virtual void tick() = 0;
        virtual void dispose() = 0;

    public:
        DebugName m_DebugName;

    protected:
        std::thread m_Thread{};
        std::atomic<bool> m_IsStopRequested, m_IsAlive;
        WaitVariable m_StopNotifier{};
        EngineTiming m_Timing{};
   };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_ENGINE_THREAD_ */

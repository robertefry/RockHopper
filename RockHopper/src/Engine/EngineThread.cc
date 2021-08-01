
#include "RockHopper/Engine/EngineThread.hh"

#include "RockHopper/Logging/Logger.hh"

#include <chrono>

/* ************************************************************************** */
// [Implementation] RockHopper::EngineTiming
/* ************************************************************************** */

namespace RockHopper
{

    void EngineTiming::init()
    {
        m_TimeLast = Clock::now();
    }

    void EngineTiming::tick()
    {
        m_TimeDelta = Clock::now() - m_TimeLast;
    }

    void EngineTiming::reduce()
    {
        m_TimeLast += m_TimeDelta.duration();
    }

    bool EngineTiming::triggered() const
    {
        return m_TimeDelta >= m_TimeOmega;
    }

} // namespace RockHopper

/* ************************************************************************** */
// [Implementation] RockHopper::EngineThread
/* ************************************************************************** */

namespace RockHopper
{

    EngineThread::~EngineThread()
    {
        m_IsStopRequested = true;
        if (m_Thread.joinable()) m_Thread.join();
    }

    EngineThread::EngineThread(std::string const& name)
        : m_DebugName{"EngineThread",name}
    {
    }

    void EngineThread::start()
    {
        std::lock_guard lock {m_Mutex};
        m_IsStopRequested = false;

        if (not m_IsAlive)
        {
            if (m_Thread.joinable())
            {
                m_Thread.join();
            }
            m_Thread = std::thread{[this]()
            {
                this->run();
            }};
        }
    }

    void EngineThread::stop()
    {
        std::lock_guard lock {m_Mutex};
        m_IsStopRequested = true;
    }

    void EngineThread::run()
    {
        ROCKHOPPER_INTERNAL_LOG_INFO("Starting {}.", m_DebugName);

        m_StartNotifier.notify_all();
        m_StopNotifier.reset();

        m_IsAlive = true;
        init();

        m_Timing.init();
        while (not m_IsStopRequested)
        {
            if (m_Timing.triggered())
            {
                m_Timing.reduce();
                tick();
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::nanoseconds{1});
            }
            m_Timing.tick();
        }

        dispose();
        m_IsAlive = false;

        m_StartNotifier.reset();
        m_StopNotifier.notify_all();

        ROCKHOPPER_INTERNAL_LOG_INFO("Stopped {}.", m_DebugName);
    }

} // namespace RockHopper

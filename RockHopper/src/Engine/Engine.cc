
#include "RockHopper/Engine/Engine.hh"

#include "RockHopper/Logging/Logger.hh"

#include <chrono>

/* ************************************************************************** */
// [Implementation] RockHopper::EngineThread
/* ************************************************************************** */

namespace RockHopper
{

    EngineThread::~EngineThread()
    {
        if (not m_IsStopRequested) stop();
        if (m_Thread.joinable()) m_Thread.join();
    }

    EngineThread::EngineThread(std::string const& name)
        : m_DebugName{"EngineThread",name}
        , m_IsStopRequested{false}
        , m_IsAlive{false}
    {
    }

    WaitVariable const& EngineThread::start()
    {
        if (not m_IsAlive)
        {
            m_Thread = std::thread([&](){
                ROCKHOPPER_INTERNAL_LOG_INFO("Starting {}.", m_DebugName);
                m_IsAlive = true;
                this->run();
                m_IsAlive = false;
                m_StopNotifier.notify_all();
                ROCKHOPPER_INTERNAL_LOG_INFO("Stopped {}.", m_DebugName);
            });
        }
        return m_StopNotifier;
    }

    WaitVariable const& EngineThread::stop()
    {
        ROCKHOPPER_INTERNAL_LOG_DEBUG("Requesting stop {}.", m_DebugName);
        m_IsStopRequested = true;
        return m_StopNotifier;
    }

    bool EngineThread::alive() const
    {
        return m_IsAlive;
    }

} // namespace RockHopper

/* ************************************************************************** */
// [Implementation] RockHopper::EngineTiming
/* ************************************************************************** */

namespace RockHopper
{

    void EngineTiming::init()
    {
        m_TimeLast = clock::now();
    }

    void EngineTiming::tick()
    {
        m_TimeSigma = clock::now() - m_TimeLast;
        if (not triggered()) std::this_thread::sleep_for(time_span{1});
    }

    void EngineTiming::reduce()
    {
        m_TimeLast += m_TimeSigma;
    }

    bool EngineTiming::triggered() const
    {
        return m_TimeSigma >= m_TimeOmega;
    }

} // namespace RockHopper

/* ************************************************************************** */
// [Implementation] RockHopper::Engine
/* ************************************************************************** */

namespace RockHopper
{

    Engine::Engine(std::string const& name)
        : EngineThread{name}
    {
    }

    void Engine::run()
    {
        init();

        m_Timing.init();
        while (not m_IsStopRequested)
        {
            m_Timing.tick();
            m_TaskQueue.execute_all();

            if (m_Timing.triggered())
            {
                m_Timing.reduce();
                tick();
            }
        }

        dispose();
    }

} // namespace RockHopper

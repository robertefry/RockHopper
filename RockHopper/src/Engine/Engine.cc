
#include "RockHopper/Engine/Engine.hh"

#include <chrono>

/* ************************************************************************** */
// [Implementation] RockHopper::EngineThread
/* ************************************************************************** */

namespace RockHopper
{

    EngineThread::EngineThread()
        : m_Thread{}
        , m_IsStopRequested{false}
        , m_IsAlive{false}
    {
    }

    EngineThread::~EngineThread()
    {
        stop();
        if (m_Thread.joinable()) m_Thread.join();
    }

    void EngineThread::start()
    {
        if (m_IsAlive) return;
        m_Thread = std::thread([&](){
            m_IsAlive = true;
            this->run();
            m_IsAlive = false;
        });
        m_IsAlive = true; // ensure m_IsAlive is set before returning.
    }

    void EngineThread::stop()
    {
        m_IsStopRequested = true;
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

    void Engine::run()
    {

        dispatch_event(EngineInitializationEvent{});
        init();
        m_Timing.init();

        while (not m_IsStopRequested)
        {
            m_Timing.tick();

            if (m_Timing.triggered())
            {
                m_Timing.reduce();
                tick();
            }
        }

        dispose();
        dispatch_event(EngineTerminationEvent{});
    }

} // namespace RockHopper

/**
 * @author Robert Fry
 * @date create 19-Jun-2021
 * @date modify 19-Jun-2021
 */

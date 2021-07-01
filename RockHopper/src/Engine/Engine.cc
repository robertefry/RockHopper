
#include "RockHopper/Engine/Engine.hh"

#include "RockHopper/Logging/Logger.hh"

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
            ROCKHOPPER_INTERNAL_LOG_INFO("Starting an EngineThread.");
            m_IsAlive = true;
            this->run();
            m_IsAlive = false;
            ROCKHOPPER_INTERNAL_LOG_INFO("Stopped an EngineThread.");
        });
        m_IsAlive = true; // ensure m_IsAlive is set before returning.
    }

    void EngineThread::stop()
    {
        ROCKHOPPER_INTERNAL_LOG_DEBUG("requesting an EngineThread stop");
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

    std::future<void> Engine::insert_task(TaskQueue<void(void)>::TaskFunc const& task)
    {
        return m_TaskQueue.push(task);
    }

    std::future<void> Engine::insert_task(TaskQueue<void(void)>::TaskFunc&& task)
    {
        return m_TaskQueue.push(std::move(task));
    }

    void Engine::run()
    {
        {
            EngineInitEvent event;
            event.engine = this;
            dispatch_event(event);
        }
        init();
        m_Timing.init();

        while (not m_IsStopRequested)
        {
            m_Timing.tick();

            if (m_Timing.triggered())
            {
                m_Timing.reduce();
                tick();

                EngineTickEvent event;
                event.engine = this;
                dispatch_event(event);
            }
            m_TaskQueue.execute_all();
        }

        dispose();
        {
            EngineDisposeEvent event;
            event.engine = this;
            dispatch_event(event);
        }
    }

} // namespace RockHopper

/**
 * @author Robert Fry
 * @date create 19-Jun-2021
 * @date modify 19-Jun-2021
 */

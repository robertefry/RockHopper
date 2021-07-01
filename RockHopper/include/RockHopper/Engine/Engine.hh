
#ifndef __HH_ROCKHOPPER_ENGINE_
#define __HH_ROCKHOPPER_ENGINE_

#include "RockHopper/Engine/EngineEvents.hh"
#include "RockHopper/Utility/TaskQueue.hh"

#include <thread>
#include <atomic>

/* ************************************************************************** */
// [Definition] RockHopper::EngineThread
/* ************************************************************************** */

namespace RockHopper
{

    class EngineThread
    {
    public:
        explicit EngineThread();
        virtual ~EngineThread();

        virtual void start();
        virtual void stop();

        bool alive() const;

    protected:
        virtual void run() = 0;

    protected:
        std::thread m_Thread;
        std::atomic<bool> m_IsStopRequested, m_IsAlive;
    };

} // namespace RockHopper

/* ************************************************************************** */
// [Definition] RockHopper::EngineTiming
/* ************************************************************************** */

namespace RockHopper
{

    class EngineTiming
    {
    public:
        using clock = std::chrono::high_resolution_clock;
        using time_point = clock::time_point;
        using time_span = std::chrono::duration<int64_t,std::nano>;

        inline void set_omega(int64_t omega) { m_TimeOmega = time_span{omega}; }
        inline auto get_omega() const -> time_span const& { return m_TimeOmega; }

        void init();
        void tick();
        void reduce();
        bool triggered() const;

    private:
        time_point m_TimeLast{};
        time_span m_TimeDelta{};
        time_span m_TimeSigma{};
        time_span m_TimeOmega{};
    };

} // namespace RockHopper

/* ************************************************************************** */
// [Definition] RockHopper::Engine
/* ************************************************************************** */

namespace RockHopper
{

    class Engine
        : public EngineThread
        , public EngineEventHandler
    {
    public:
        explicit Engine() = default;
        virtual ~Engine() = default;

        inline auto timing() -> EngineTiming& { return m_Timing; }

        std::future<void> insert_task(TaskQueue<void(void)>::TaskFunc const&);
        std::future<void> insert_task(TaskQueue<void(void)>::TaskFunc&&);

    protected:
        virtual void init() = 0;
        virtual void tick() = 0;
        virtual void dispose() = 0;

    private:
        virtual void run() override final;

    protected:
        EngineTiming m_Timing{};
        TaskQueue<void(void)> m_TaskQueue{};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_ENGINE_ */

/**
 * @author Robert Fry
 * @date create 19-Jun-2021
 * @date modify 19-Jun-2021
 */

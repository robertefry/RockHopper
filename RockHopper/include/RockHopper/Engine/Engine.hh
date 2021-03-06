
#ifndef __HH_ROCKHOPPER_ENGINE_
#define __HH_ROCKHOPPER_ENGINE_

#include "RockHopper/Debug.hh"
#include "RockHopper/Utility/TaskQueue.hh"
#include "RockHopper/Utility/WaitVariable.hh"

#include <atomic>
#include <thread>

/* ************************************************************************** */
// [Definition] RockHopper::EngineThread
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

        bool alive() const;

    protected:
        virtual void run() = 0;

    public:
        DebugName m_DebugName;

    protected:
        std::thread m_Thread{};
        std::atomic<bool> m_IsStopRequested, m_IsAlive;
        WaitVariable m_StopNotifier{};
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

    class Engine : public EngineThread
    {
    public:
        virtual ~Engine() = default;
        explicit Engine(std::string const& name);

        inline auto timing() -> EngineTiming& { return m_Timing; }

        template <typename T_Func, typename... T_Args>
        auto insert_task(T_Func&& func, T_Args&&... args);

    protected:
        virtual void init() = 0;
        virtual void tick() = 0;
        virtual void dispose() = 0;

    private:
        virtual void run() override final;

    protected:
        EngineTiming m_Timing{};
        TaskQueue m_TaskQueue{};
    };

    template <typename T_Func, typename... T_Args>
    auto Engine::insert_task(T_Func&& func, T_Args&&... args)
    {
        return m_TaskQueue.push_task(
            std::forward<T_Func>(func), std::forward<T_Args>(args)...
        );
    }

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_ENGINE_ */

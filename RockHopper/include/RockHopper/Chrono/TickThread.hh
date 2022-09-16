
#ifndef __HH_ROCKHOPPER_CHRONO_TICK_THREAD_
#define __HH_ROCKHOPPER_CHRONO_TICK_THREAD_

#include "RockHopper/Event/EventHandler.hh"
#include "RockHopper/Chrono/TickEvents.hh"

#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>

/* ************************************************************************** */
// [Definition] RockHopper::Chrono::TickThread
/* ************************************************************************** */

namespace RockHopper::Chrono
{

    struct I_TickThread
        : virtual Event::I_EventHandler<TickEvent>
        , protected virtual TickEvent::ListenerType
    {
        using Clock = std::chrono::high_resolution_clock;

    public:
        virtual void start() = 0;
        virtual void stop() = 0;

        virtual bool is_alive() const = 0;

        virtual void set_delta(Clock::duration const&) = 0;
        virtual auto get_delta() const -> Clock::duration = 0;
    };

    template <typename T_EventDispatcher>
    class TickThread
        : public virtual I_TickThread
        , public virtual Event::EventHandler<TickEvent,T_EventDispatcher>
    {
    public:
        using EventHandler = Event::EventHandler<TickEvent,T_EventDispatcher>;
        using Clock = I_TickThread::Clock;

        virtual ~TickThread();
        explicit TickThread();

        TickThread(TickThread&&) = delete;
        TickThread& operator=(TickThread&&) = delete;

        virtual void start() override;
        virtual void stop() override;

        virtual bool is_alive() const override { return m_IsAlive; };

        virtual void set_delta(Clock::duration const&) override;
        virtual auto get_delta() const -> Clock::duration override;

    private:
        using EventHandler::dispatch_event;

    private:
        void run();

    private:
        std::thread m_Thread{};
        std::mutex m_ThreadMutex{}; // control thread lifetime

        Clock::duration m_TimeDelta{};

        std::atomic<bool> m_IsAlive = false;
        std::atomic<bool> m_IsStopRequested = false;
    };

} // namespace RockHopper::Chrono

/* ************************************************************************** */
// [Implementation] RockHopper::Chrono::TickThread
/* ************************************************************************** */

namespace RockHopper::Chrono
{

    template <typename T_EventDispatcher>
    TickThread<T_EventDispatcher>::~TickThread()
    {
        m_IsStopRequested = true;
        if (m_Thread.joinable()) m_Thread.join();
    }

    template <typename T_EventDispatcher>
    TickThread<T_EventDispatcher>::TickThread()
    {
    }

    template <typename T_EventDispatcher>
    void TickThread<T_EventDispatcher>::start()
    {
        std::unique_lock lock {m_ThreadMutex};
        m_IsStopRequested = false;

        if (not m_IsAlive)
        {
            if (m_Thread.joinable()) m_Thread.join();
            m_Thread = std::thread{[this](){ this->run(); }};
        }
    }

    template <typename T_EventDispatcher>
    void TickThread<T_EventDispatcher>::stop()
    {
        std::unique_lock lock {m_ThreadMutex};
        m_IsStopRequested = true;
    }

    template <typename T_EventDispatcher>
    void TickThread<T_EventDispatcher>::set_delta(Clock::duration const& delta)
    {
        m_TimeDelta = delta;
    }

    template <typename T_EventDispatcher>
    auto TickThread<T_EventDispatcher>::get_delta() const -> Clock::duration
    {
        return m_TimeDelta;
    }

    template <typename T_EventDispatcher>
    void TickThread<T_EventDispatcher>::run()
    {
        m_IsAlive = true;

        {
            auto event = TickEvent_Init{};
            event.accept(this); // is this->I_TickEvent::on_event(event)
            dispatch_event(event);
        }

        Clock::time_point time_last = Clock::now();
        Clock::duration time_delta;

        while (not m_IsStopRequested)
        {
            time_delta = Clock::now() - time_last;

            if (time_delta >= m_TimeDelta)
            {
                time_last += time_delta;

                auto event = TickEvent_OnTick{};
                event.accept(this); // is this->I_TickEvent::on_event(event)
                dispatch_event(event);
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::nanoseconds{1});
            }
        }

        {
            auto event = TickEvent_Dispose{};
            event.accept(this); // is this->I_TickEvent::on_event(event)
            dispatch_event(event);
        }

        m_IsAlive = false;
    }

} // namespace RockHopper::Chrono

#endif /* __HH_ROCKHOPPER_CHRONO_TICK_THREAD_ */

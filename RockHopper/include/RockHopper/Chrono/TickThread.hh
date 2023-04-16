
#ifndef ROCKHOPPER_CHRONO_TICK_THREAD_HH
#define ROCKHOPPER_CHRONO_TICK_THREAD_HH

#include "RockHopper/Chrono/TickThread.Events.hh"

#include "RockHopper/Event/EventHandler.hh"

#include <chrono>
#include <thread>

namespace RockHopper::Chrono
{

    template <typename T_Dispatcher>
    class TickThread
        : public Event::EventHandler<TickEvent,T_Dispatcher>
        , protected TickEvent::Listener
    {
        using Dispatcher = T_Dispatcher;
        using EventHandler = Event::EventHandler<TickEvent,Dispatcher>;

    public:
        using Clock = std::chrono::system_clock;

        virtual ~TickThread();

        void start();
        void stop();

        bool is_alive() const;

        void set_delta(Clock::duration const&);
        auto get_delta() const -> Clock::duration;

    protected:
        template <typename T_Event>
        void dispatch_event(T_Event&&);

    private:
        void run();

    private:
        Clock::duration m_Delta{};

        std::thread m_Thread{};
        std::atomic<bool> m_IsAlive = false;
        std::atomic<bool> m_IsStopRequested = false;
    };

    template <typename T_Dispatcher>
    TickThread<T_Dispatcher>::~TickThread()
    {
        m_IsStopRequested = true;
        if (m_Thread.joinable()) m_Thread.join();
    }

    template <typename T_Dispatcher>
    void TickThread<T_Dispatcher>::start()
    {
        m_IsStopRequested = false;

        if (not m_IsAlive)
        {
            if (m_Thread.joinable()) m_Thread.join();
            m_Thread = std::thread{[this]{ this->run(); }};
        }
    }

    template <typename T_Dispatcher>
    void TickThread<T_Dispatcher>::stop()
    {
        m_IsStopRequested = true;
    }

    template <typename T_Dispatcher>
    bool TickThread<T_Dispatcher>::is_alive() const
    {
        return m_IsAlive;
    }

    template <typename T_Dispatcher>
    void TickThread<T_Dispatcher>::set_delta(Clock::duration const& delta)
    {
        m_Delta = delta;
    }

    template <typename T_Dispatcher>
    auto TickThread<T_Dispatcher>::get_delta() const -> Clock::duration
    {
        return m_Delta;
    }

    template <typename T_Dispatcher>
    template <typename T_Event>
    void TickThread<T_Dispatcher>::dispatch_event(T_Event&& event)
    {
        this->on_event(event);
        this->EventHandler::dispatch_event(std::forward<T_Event>(event));
    }

    template <typename T_Dispatcher>
    void TickThread<T_Dispatcher>::run()
    {
        m_IsAlive = true;
        dispatch_event(TickEvent_Init{});

        Clock::time_point time_last = Clock::now();
        Clock::duration time_delta;

        while (not m_IsStopRequested)
        {
            time_delta = Clock::now() - time_last;
            if (time_delta < m_Delta)
            {
                std::this_thread::sleep_for(std::chrono::nanoseconds{1});
                continue;
            }

            time_last += time_delta;

            dispatch_event(TickEvent_OnTick{time_delta});
        }

        m_IsAlive = false;
        dispatch_event(TickEvent_Dispose{});
    }

} // namespace RockHopper::Chrono

#endif /* ROCKHOPPER_CHRONO_TICK_THREAD_HH */

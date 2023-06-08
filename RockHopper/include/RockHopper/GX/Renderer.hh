
#ifndef ROCKHOPPER_GX_RENDERER_HH
#define ROCKHOPPER_GX_RENDERER_HH

#include "RockHopper/Chrono/TickThread.hh"
#include "RockHopper/Event/EventDispatch.Sequential.hh"

#include "RockHopper/Util/Singleton.hh"
#include "RockHopper/Util/TaskQueue.hh"
#include "RockHopper/Util/Lifetime.hh"

namespace RockHopper::GX
{

    /*
     * The Renderer is a singleton encapsulation of a task thread, which simply
     * churns away at tasks. The thread is started-up and shut-down on demand
     * dependent on whether the renderer is being used or not.
     */
    class Renderer
    {
        class Thread : private Util::NoMove
            , private Chrono::TickThread<Event::Dispatch::Sequential>
        {
            friend Renderer;
            using TickThread = Chrono::TickThread<Event::Dispatch::Sequential>;

        public:
            virtual ~Thread();
            explicit Thread();

        private:
            using TickThread::on_event;
            void on_event(Chrono::TickEvent_Init const&) override;
            void on_event(Chrono::TickEvent_Dispose const&) override;
            void on_event(Chrono::TickEvent_OnTick const&) override;

        private:
            Util::TaskQueue m_TaskQueue{};
            Util::TaskQueue::Consumer m_Consumer = m_TaskQueue.make_consumer();
        };

    public:

        template <typename T_Func, typename... T_Args>
        [[nodiscard]] auto push_task(T_Func&& func, T_Args&&... args)
        {
            if (m_Thread->alive_id() == std::this_thread::get_id())
            {
                return m_Thread->m_TaskQueue.execute_task(
                    std::forward<T_Func>(func), std::forward<T_Args>(args)...
                );
            }

            return m_Producer.push_task(
                std::forward<T_Func>(func), std::forward<T_Args>(args)...
            );
        }

    private:
        Util::Singleton<Thread> m_Thread{};
        Util::TaskQueue::Producer m_Producer = m_Thread->m_TaskQueue.make_producer();
    };

} // namespace RockHopper::GX

#endif /* ROCKHOPPER_GX_RENDERER_HH */

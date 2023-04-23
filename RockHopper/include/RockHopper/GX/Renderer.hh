
#ifndef ROCKHOPPER_GX_RENDERER_HH
#define ROCKHOPPER_GX_RENDERER_HH

#include "RockHopper/Chrono/TickThread.hh"
#include "RockHopper/Event/EventDispatch.Sequential.hh"

#include "RockHopper/Util/Singleton.hh"
#include "RockHopper/Util/TaskQueue.hh"

namespace RockHopper::GX
{

    /*
     * The Renderer is a singleton encapsulation of a task thread, which simply
     * churns away at tasks. The thread is started-up and shut-down on demand
     * dependent on whether the renderer is being used or not.
     */
    class Renderer
    {
        struct Thread
            : private Chrono::TickThread<Event::Dispatch::Sequential>
        {
            using TickThread = Chrono::TickThread<Event::Dispatch::Sequential>;

            virtual ~Thread();
            explicit Thread();

            Thread(Thread const&) = delete;

            void on_event(Chrono::TickEvent_Init const&);
            void on_event(Chrono::TickEvent_Dispose const&);
            void on_event(Chrono::TickEvent_OnTick const&);

            static constexpr size_t s_TaskQueueInitialCapacity = 1024;
            Util::TaskQueue m_TaskQueue {s_TaskQueueInitialCapacity};
        };

    public:

        /*
         * reserve a number, n, of nodes for the task queue
         */
        inline void reserve(size_t n)
        {
            m_Thread->m_TaskQueue.reserve(n);
        }

        /*
         * push a task to be run on the renderer thread
         */
        template <typename T_Func, typename... T_Args>
        [[nodiscard]] auto push_task(T_Func&& func, T_Args&&... args)
        {
            return m_Thread->m_TaskQueue.push_task(
                std::forward<T_Func>(func), std::forward<T_Args>(args)...
            );
        }

    private:
        Util::Singleton<Thread> m_Thread{};
    };

} // namespace RockHopper::GX

#endif /* ROCKHOPPER_GX_RENDERER_HH */

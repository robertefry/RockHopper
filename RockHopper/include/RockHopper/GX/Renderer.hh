
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
            , public Chrono::TickThread<Event::Dispatch::Sequential>
        {
            using TickThread = Chrono::TickThread<Event::Dispatch::Sequential>;

        public:
            virtual ~Thread();
            explicit Thread();

            template <typename T_Func, typename... T_Args>
            [[nodiscard]] auto push_task(T_Func&& func, T_Args&&... args)
            {
                if (alive_id() == std::this_thread::get_id())
                {
                    return m_TaskQueue.execute_task(
                        std::forward<T_Func>(func), std::forward<T_Args>(args)...
                    );
                } else {
                    return m_TaskQueue.push_task(
                        std::forward<T_Func>(func), std::forward<T_Args>(args)...
                    );
                }
            }

        private:
            void on_event(Chrono::TickEvent_Init const&);
            void on_event(Chrono::TickEvent_Dispose const&);
            void on_event(Chrono::TickEvent_OnTick const&);

        private:
            static constexpr size_t s_TaskQueueInitialCapacity = 1024;
            Util::TaskQueue m_TaskQueue {s_TaskQueueInitialCapacity};
        };

    public:
        auto operator->() & -> auto& { return m_Thread; }
        auto operator->() const& -> auto const& { return m_Thread; }

    private:
        Util::Singleton<Thread> m_Thread{};
    };

} // namespace RockHopper::GX

#endif /* ROCKHOPPER_GX_RENDERER_HH */

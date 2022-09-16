
#ifndef __HH_ROCKHOPPER_GX_RENDERER_
#define __HH_ROCKHOPPER_GX_RENDERER_

#include "RockHopper/Chrono/TickThread.hh"

#include <thread-tools/task_queue.hh>

#include <mutex>

/* ************************************************************************** */
// [Definition] RockHopper::GX::Renderer
/* ************************************************************************** */

namespace RockHopper::GX
{

    class Renderer
    {
        using TickThread = Chrono::TickThread<Event::Dispatch::SequentialDispatch>;
        using TaskQueue = thd::task_queue;

    public:
        virtual ~Renderer();
        explicit Renderer();

        Renderer(Renderer const&);
        Renderer& operator=(Renderer const&);

        Renderer(Renderer&&);
        Renderer& operator=(Renderer&&);

    public:
        template <typename Func, typename... Args>
        auto push_task(Func&& func, Args&&... args);
        template <typename Func, typename... Args>
        auto wait_task(Func&& func, Args&&... args);

    private:
        struct Impl // keep private for s_UseCount correctness
        {
            TickThread tick_thread{};
            TaskQueue task_queue{};
        };
        static inline Impl* s_Impl = nullptr; // destructible singleton
        static inline std::size_t s_UseCount = 0; // new/del s_Impl on first use count
        static inline std::mutex s_Mutex{}; // for s_UseCount
    };

} // namespace RockHopper::GX

/* ************************************************************************** */
// [Implementation] RockHopper::GX::Renderer
/* ************************************************************************** */

namespace RockHopper::GX
{

    template <typename Func, typename... Args>
    auto Renderer::push_task(Func&& func, Args&&... args)
    {
        return s_Impl->task_queue.push_task(
            std::forward<Func>(func), std::forward<Args>(args)...
        );
    }

    template <typename Func, typename... Args>
    auto Renderer::wait_task(Func&& func, Args&&... args)
    {
        return s_Impl->task_queue.wait_task(
            std::forward<Func>(func), std::forward<Args>(args)...
        );
    }

} // namespace RockHopper::GX

#endif /* __HH_ROCKHOPPER_GX_RENDERER_ */

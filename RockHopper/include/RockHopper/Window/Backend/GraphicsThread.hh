
#ifndef __HH_ROCKHOPPER_WINDOW_GRAPHICS_THREAD_
#define __HH_ROCKHOPPER_WINDOW_GRAPHICS_THREAD_

#include "RockHopper/Engine/Engine.hh"
#include "RockHopper/Utility/TaskQueue.hh"

#include <atomic>

namespace RockHopper
{

    class GraphicsThread
    {
        class Instance : public EngineThread, public TaskQueue
        {
        public:
            virtual ~Instance() = default;
            explicit Instance() : EngineThread{"GraphicsThread::Instance"} {}
        private:
            virtual void run() override;
        };

    public:
        virtual ~GraphicsThread();
        explicit GraphicsThread();

        explicit GraphicsThread(GraphicsThread const&);
        GraphicsThread& operator=(GraphicsThread const&);

        template <typename T_Func, typename... T_Args>
        auto push_task(T_Func&& func, T_Args&&... args)
            -> std::future<typename std::invoke_result<T_Func,T_Args...>::type>;
        template <typename T_Func, typename... T_Args>
        auto wait_task(T_Func&& func, T_Args&&... args)
            -> std::invoke_result<T_Func,T_Args...>::type;

    private:
        static inline Instance* s_InstancePtr{};
        static inline std::atomic<unsigned> s_NumInstances = 0;
    };

    template <typename T_Func, typename... T_Args>
    auto GraphicsThread::push_task(T_Func&& func, T_Args&&... args)
        -> std::future<typename std::invoke_result<T_Func,T_Args...>::type>
    {
        return s_InstancePtr->push_task(
            std::forward<T_Func>(func), std::forward<T_Args>(args)...
        );
    }

    template <typename T_Func, typename... T_Args>
    auto GraphicsThread::wait_task(T_Func&& func, T_Args&&... args)
        -> std::invoke_result<T_Func,T_Args...>::type
    {
        return s_InstancePtr->wait_task(
            std::forward<T_Func>(func), std::forward<T_Args>(args)...
        );
    }

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WINDOW_GRAPHICS_THREAD_ */

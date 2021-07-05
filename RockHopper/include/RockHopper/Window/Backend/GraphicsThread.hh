
#ifndef __HH_ROCKHOPPER_WINDOW_GRAPHICS_THREAD_
#define __HH_ROCKHOPPER_WINDOW_GRAPHICS_THREAD_

#include "RockHopper/Engine/Engine.hh"
#include "RockHopper/Utility/TaskQueue.hh"

#include <atomic>

namespace RockHopper
{

    class GraphicsThread
    {
        class Instance : public EngineThread, public TaskQueue<void(void)>
        {
            virtual void run() override;
        };

    public:
        virtual ~GraphicsThread();
        explicit GraphicsThread();

        explicit GraphicsThread(GraphicsThread const&);
        GraphicsThread& operator=(GraphicsThread const&);

        inline auto push_task(Instance::TaskFunc const& task) { return s_InstancePtr->push_task(task); }
        inline auto push_task(Instance::TaskFunc&& task) { return s_InstancePtr->push_task(std::move(task)); }

        inline auto wait_task(Instance::TaskFunc const& task) { return s_InstancePtr->wait_task(task); }
        inline auto wait_task(Instance::TaskFunc&& task) { return s_InstancePtr->wait_task(std::move(task)); }

    private:
        static inline Instance* s_InstancePtr{};
        static inline std::atomic<unsigned> s_NumInstances = 0;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WINDOW_GRAPHICS_THREAD_ */

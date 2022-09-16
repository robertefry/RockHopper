
#include "RockHopper/GX/Renderer.hh"

#include "RockHopper/Event/EventListener.hh"
#include "RockHopper/Debug.hh"

namespace RockHopper::GX
{

    Renderer::~Renderer()
    {
        std::unique_lock lock {s_Mutex};

        if (s_UseCount == 1)
        {
            // Push a final task to the task_queue as a marker, to clear all tasks
            // before shutting down the tick_thread. When this task completes,
            // there should be no further tasks in the task_queue.
            s_Impl->task_queue.wait_task([]()
            {
            });
            ROCKHOPPER_INTERNAL_ASSERT_WARN(s_Impl->task_queue.empty(),
                "Renderer queue not empty after shutdown" );

            // Stop the tick_thread
            s_Impl->tick_thread.stop();

            // Delete the s_Impl pointer after the final user destruction
            delete s_Impl;
        }
        --s_UseCount;
    }

    Renderer::Renderer()
    {
        std::unique_lock lock {s_Mutex};

        if (s_UseCount == 0)
        {
            // Create the s_Impl pointer before the first user construction
            s_Impl = new Impl;

            // Insert an event listener to the tick_thread, which will execute
            // all tasks in the task_queue every TickEvent_OnTick.
            s_Impl->tick_thread.insert_event_listener(
                Event::MakeListener<Chrono::TickEvent_OnTick>([this](auto)
            {
                s_Impl->task_queue.execute_all();
            }));

            // Insert event listeners to log when the tick_thread is booting up
            // and is shutting down.
            s_Impl->tick_thread.insert_event_listener(
                Event::MakeListener<Chrono::TickEvent_Init>([this](auto)
            {
                ROCKHOPPER_INTERNAL_LOG_INFO("Renderer starting up");
            }));
            s_Impl->tick_thread.insert_event_listener(
                Event::MakeListener<Chrono::TickEvent_Dispose>([this](auto)
            {
                ROCKHOPPER_INTERNAL_LOG_INFO("Renderer shutting down");
            }));

            // Start the tick_thread
            s_Impl->tick_thread.start();
        }
        ++s_UseCount;
    }

    Renderer::Renderer(Renderer const&)
    {
        std::unique_lock lock {s_Mutex};

        ++s_UseCount;
    }

    Renderer& Renderer::operator=(Renderer const&)
    {
        // nop
        return *this;
    }

    Renderer::Renderer(Renderer&&)
    {
        std::unique_lock lock {s_Mutex};

        ++s_UseCount;
    }

    Renderer& Renderer::operator=(Renderer&&)
    {
        // nop
        return *this;
    }

} // namespace RockHopper::GX

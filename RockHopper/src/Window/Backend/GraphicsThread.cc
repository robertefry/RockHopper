
#include "RockHopper/Window/Backend/GraphicsThread.hh"

#include "RockHopper/Logging/Logger.hh"

#include <chrono>

namespace RockHopper
{

    void GraphicsThread::Instance::run()
    {
        while (not m_IsStopRequested)
        {
            execute_all();
            insert_notifier().wait_for(std::chrono::seconds{1});
        }
    }

    GraphicsThread::~GraphicsThread()
    {
        if (s_NumInstances == 1)
        {
            ROCKHOPPER_INTERNAL_LOG_DEBUG("Stopping the current graphics thead.");
            WaitVariable const& stop = s_InstancePtr->stop();
            while (s_InstancePtr->alive()) { stop.wait_for(std::chrono::seconds{1}); }
            delete s_InstancePtr;
        }
        s_NumInstances -= 1;
    }

    GraphicsThread::GraphicsThread()
    {
        if (s_NumInstances == 0)
        {
            ROCKHOPPER_INTERNAL_LOG_DEBUG("Starting a new graphics thead.");
            s_InstancePtr = new Instance{};
            s_InstancePtr->start();
        }
        s_NumInstances += 1;
    }

    GraphicsThread::GraphicsThread(GraphicsThread const&)
    {
        s_NumInstances += 1;
    }

    GraphicsThread& GraphicsThread::operator=(GraphicsThread const&)
    {
        s_NumInstances += 1;
        return *this;
    }

} // namespace RockHopper

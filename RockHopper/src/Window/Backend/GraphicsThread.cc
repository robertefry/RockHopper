
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
            std::this_thread::sleep_for(std::chrono::nanoseconds{1});
        }
    }

    GraphicsThread::~GraphicsThread()
    {
        if (s_NumInstances == 1)
        {
            s_InstancePtr->stop().wait();
            delete s_InstancePtr;
        }
        s_NumInstances -= 1;
    }

    GraphicsThread::GraphicsThread()
    {
        if (s_NumInstances == 0)
        {
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

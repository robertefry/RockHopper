
#include "RockHopper/Rendering/Renderer.hh"
#include "RockHopper/Rendering/Platform/OpenGL_Renderer.hh"

#include "RockHopper/Debug.hh"

#include <chrono>

namespace RockHopper
{

    void RenderThread::Instance::run()
    {
        while (not m_IsStopRequested)
        {
            execute_all();
            insert_notifier().wait_for(std::chrono::seconds{1});
        }
    }

    RenderThread::~RenderThread()
    {
        if (s_NumInstances == 1)
        {
            ROCKHOPPER_INTERNAL_LOG_DEBUG("Stopping the current render thead.");
            WaitVariable const& stop = s_InstancePtr->stop();
            while (s_InstancePtr->alive()) { stop.wait_for(std::chrono::seconds{1}); }
            delete s_InstancePtr;
        }
        s_NumInstances -= 1;
    }

    RenderThread::RenderThread()
    {
        if (s_NumInstances == 0)
        {
            ROCKHOPPER_INTERNAL_LOG_DEBUG("Starting a new render thead.");
            s_InstancePtr = new Instance{};
            s_InstancePtr->start();
        }
        s_NumInstances += 1;
    }

    RenderThread::RenderThread(RenderThread const&)
    {
        s_NumInstances += 1;
    }

    RenderThread& RenderThread::operator=(RenderThread const&)
    {
        s_NumInstances += 1;
        return *this;
    }

    void Renderer::Create(API api)
    {
        if (s_Instance != nullptr)
        {
            ROCKHOPPER_INTERNAL_LOG_FATAL("Cannot create multiple renderers!");
        }
        s_RenderAPI = api;

        s_Instance = [&]() -> std::unique_ptr<Renderer>
        {
            switch (api)
            {
                case API::OpenGL: {
                    return std::make_unique<OpenGL_Renderer>();
                } break;
                case API::None: {
                    ROCKHOPPER_INTERNAL_LOG_FATAL("Cannot create a renderer from the None API!");
                } break;
                default: {
                    ROCKHOPPER_INTERNAL_LOG_FATAL("Could not create a renderer from an unknown API!");
                } break;
            }
            return nullptr;
        }();
    }

} // namespace RockHopper

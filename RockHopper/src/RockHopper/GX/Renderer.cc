
#include "RockHopper/GX/Renderer.hh"

#include "RockHopper/Logging/LoggerCore.enable.hh"

namespace RockHopper::GX
{

    Renderer::Thread::~Thread()
    {
        TickThread::stop();
        while (is_alive()) { /* spin */ }
    }

    Renderer::Thread::Thread()
    {
        TickThread::start();
        while (not is_alive()) { /* spin */ }
    }

    void Renderer::Thread::on_event(Chrono::TickEvent_Init const&)
    {
        ROCKHOPPER_LOG_INFO("Renderer starting up");
    }

    void Renderer::Thread::on_event(Chrono::TickEvent_Dispose const&)
    {
        ROCKHOPPER_LOG_INFO("Renderer shutting down");
    }

    void Renderer::Thread::on_event(Chrono::TickEvent_OnTick const&)
    {
        if (not m_Consumer.empty()) m_Consumer.execute_one();
    }

} // namespace RockHopper::GX

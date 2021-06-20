
#include "Sandbox.hh"

#include "RockHopper/Logging/Logger.hh"
#include "RockHopper/Engine/Engine.hh"

Sandbox::Sandbox()
{
}

Sandbox::~Sandbox()
{
}

void Sandbox::run()
{
    class TestEngine
        : public RockHopper::Engine
    {
        void init() override
        {
            ROCKHOPPER_LOG_INFO(__PRETTY_FUNCTION__);
        }
        void tick() override
        {
            ROCKHOPPER_LOG_INFO(__PRETTY_FUNCTION__);
            if (++m_TickCount >= 3) stop();
        }
        void dispose() override
        {
            ROCKHOPPER_LOG_INFO(__PRETTY_FUNCTION__);
        }
    private:
        unsigned int m_TickCount = 0;
    };

    TestEngine test_engine;
    test_engine.timing().set_omega(1'000'000'000);
    test_engine.start();
    while (test_engine.alive()); // Keep the main thread alive
}

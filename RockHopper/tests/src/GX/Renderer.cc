
#include "RockHopper/GX/Renderer.hh"
using namespace RockHopper::GX;

#include "RockHopper/Logging/Logger.hh"

#include <gtest/gtest.h>

TEST(GX,RENDERER)
{
    std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>();

    renderer->push_task([]()
    {
        ROCKHOPPER_LOG_INFO("Renderer working great :)");
    });
}


#include <catch2/catch_all.hpp>

// enable the core logger
#include "RockHopper/Logging/LoggerCore.enable.hh"

TEST_CASE("Core Logger hash-defines are working.")
{
    ROCKHOPPER_LOG_INFO("The LoggerCore logger is working.");
}

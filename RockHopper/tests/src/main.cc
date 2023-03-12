
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <catch2/catch_all.hpp>

TEST_CASE("Are all dependencies working?")
{
    auto message = fmt::format("{}","Dependencies are working; Catch2, FmtLib, SpdLog");
    spdlog::info(message);
}


#include <gtest/gtest.h>

#include "RockHopper/RockHopper.hh"

class TestApp : public RockHopper::Application
{
public:
    virtual ~TestApp() = default;
    explicit TestApp() = default;

    virtual int run() override;
};

int TestApp::run()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

/*
 * main function implemented in RockHopper.cpp
 */

// Required by RockHopper engine
RockHopper::Application* RockHopper::CreateApplication()
{
    return new TestApp{};
}

TEST(MAIN,TESTING)
{
    std::cout << "GTest tests are working :)" << "\n";
}

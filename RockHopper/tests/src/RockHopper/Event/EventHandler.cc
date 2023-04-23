
#include <catch2/catch_all.hpp>

#include "RockHopper/Event/Event.Listeners.hh"
#include "RockHopper/Event/EventHandler.hh"
#include "RockHopper/Event/EventDispatch.Sequential.hh"

/* ************************************************************************** */

struct TestEvent1 { std::unique_ptr<int> no_copy; };
struct TestEvent2 { std::unique_ptr<int> no_copy; };
using TestEvent = RockHopper::Event::EventSet<TestEvent1,TestEvent2>;

/* ************************************************************************** */

TEST_CASE("EventHandler can correctly dispatch events")
{
    using Dispatcher = RockHopper::Event::Dispatch::Sequential;
    using EventHandler = RockHopper::Event::EventHandler<TestEvent,Dispatcher>;

    EventHandler event_handler;

    size_t count = 0;
    event_handler.emplace_event_listener(
        TestEvent::ListenerKit<TestEvent1>::Make([&]<typename E>(E&&)
    {
        count += 1;
    }));

    REQUIRE(count == 0);

    event_handler.dispatch_event(TestEvent1{});

    REQUIRE(count == 1);
}

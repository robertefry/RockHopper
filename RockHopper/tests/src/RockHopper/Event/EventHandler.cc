
#include <catch2/catch_all.hpp>

#include "RockHopper/Event/Event.Listeners.hh"
#include "RockHopper/Event/EventHandler.hh"
#include "RockHopper/Event/EventDispatch.Sequential.hh"

using namespace RockHopper;

/* ************************************************************************** */

struct TestEvent1 { std::unique_ptr<int> no_copy; };
struct TestEvent2 { std::unique_ptr<int> no_copy; };
using TestEvent = Event::EventSet<TestEvent1,TestEvent2>;

struct TestEventHandler : public Event::EventHandler<TestEvent,Event::Dispatch::Sequential>
{
    using EventHandler = Event::EventHandler<TestEvent,Event::Dispatch::Sequential>;
    using EventHandler::dispatch_event;
};

/* ************************************************************************** */

TEST_CASE("EventHandler can correctly dispatch events")
{
    TestEventHandler event_handler;

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


#include <catch2/catch_all.hpp>

#include "RockHopper/Event/Event.hh"
#include "RockHopper/Event/Event.Listeners.hh"

/* ************************************************************************** */

struct TestEvent1 { std::unique_ptr<int> no_copy; };
struct TestEvent2 { std::unique_ptr<int> no_copy; };
using TestEvent = RockHopper::Event::EventSet<TestEvent1,TestEvent2>;

/* ************************************************************************** */

TEST_CASE("EventSet::ListenerKit<...>::Make correctly makes an event listener")
{
    size_t count = 0;

    auto listener = TestEvent::ListenerKit<TestEvent1>::Make([&]<typename E>(E&& event)
    {
        count += 1;
    });

    REQUIRE(count == 0);

    TestEvent::Variant const event = TestEvent1{};
    TestEvent::Dispatch(listener,event);

    REQUIRE(count == 1);
}

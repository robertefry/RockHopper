
#include <catch2/catch_all.hpp>

#include "RockHopper/Event/Event.hh"
#include "RockHopper/Event/Event.Events.hh"

/* ************************************************************************** */

struct TestEvent1 { std::unique_ptr<int> no_copy; };
struct TestEvent2 { std::unique_ptr<int> no_copy; };
using TestEvent = RockHopper::Event::EventSet<TestEvent1,TestEvent2>;

/* ************************************************************************** */

TEST_CASE("EventSet::EventKit<...>::Make correctly makes an instance of EventSet::Variant")
{
    auto event = TestEvent::EventKit<TestEvent1>::Make();
    REQUIRE(std::is_same<TestEvent::Variant,std::remove_cv<decltype(event)>::type>::value);
}

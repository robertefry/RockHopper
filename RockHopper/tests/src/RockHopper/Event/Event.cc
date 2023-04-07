
#include "RockHopper/Event/Event.hh"

#include <catch2/catch_all.hpp>
#include <unordered_map>

enum ArgumentQualifier
{
    VAL, L_REF, CL_REF, R_REF, CR_REF,
};

/* ************************************************************************** */

struct TestEvent1 {};
struct TestEvent2 {};
using TestEvent = RockHopper::Event::EventSet<TestEvent1,TestEvent2>;

template <typename T_Event>
struct TestEvent_Listener : TestEvent::Listener
{
    std::unordered_map<ArgumentQualifier,size_t> counts {};

    void on_event(T_Event const&) override { counts[CL_REF] += 1; }
    void on_event(T_Event &) override { counts[L_REF] += 1; }
    void on_event(T_Event const&&) override { counts[CR_REF] += 1; }
    void on_event(T_Event &&) override { counts[R_REF] += 1; }

    using TestEvent::Listener::on_event;
};

/* ************************************************************************** */

TEST_CASE("EventSet::Dispatch correctly dispatches an event to a listener")
{
    SECTION("listen for const& events")
    {
        TestEvent_Listener<TestEvent1> listener;

        REQUIRE(listener.counts[VAL] == 0);
        REQUIRE(listener.counts[L_REF] == 0);
        REQUIRE(listener.counts[CL_REF] == 0);
        REQUIRE(listener.counts[R_REF] == 0);
        REQUIRE(listener.counts[CR_REF] == 0);

        TestEvent::Variant const event = TestEvent1{};
        TestEvent::Dispatch(listener,event);

        REQUIRE(listener.counts[VAL] == 0);
        REQUIRE(listener.counts[L_REF] == 0);
        REQUIRE(listener.counts[CL_REF] == 1);
        REQUIRE(listener.counts[R_REF] == 0);
        REQUIRE(listener.counts[CR_REF] == 0);
    }

    SECTION("listen for & events")
    {
        TestEvent_Listener<TestEvent1> listener;

        REQUIRE(listener.counts[VAL] == 0);
        REQUIRE(listener.counts[L_REF] == 0);
        REQUIRE(listener.counts[CL_REF] == 0);
        REQUIRE(listener.counts[R_REF] == 0);
        REQUIRE(listener.counts[CR_REF] == 0);

        TestEvent::Variant event = TestEvent1{};
        TestEvent::Dispatch(listener,event);

        REQUIRE(listener.counts[VAL] == 0);
        REQUIRE(listener.counts[L_REF] == 1);
        REQUIRE(listener.counts[CL_REF] == 0);
        REQUIRE(listener.counts[R_REF] == 0);
        REQUIRE(listener.counts[CR_REF] == 0);
    }

    SECTION("listen for const&& events")
    {
        TestEvent_Listener<TestEvent1> listener;

        REQUIRE(listener.counts[VAL] == 0);
        REQUIRE(listener.counts[L_REF] == 0);
        REQUIRE(listener.counts[CL_REF] == 0);
        REQUIRE(listener.counts[R_REF] == 0);
        REQUIRE(listener.counts[CR_REF] == 0);

        TestEvent::Variant const event = TestEvent1{};
        TestEvent::Dispatch(listener,std::move(event));

        REQUIRE(listener.counts[VAL] == 0);
        REQUIRE(listener.counts[L_REF] == 0);
        REQUIRE(listener.counts[CL_REF] == 0);
        REQUIRE(listener.counts[R_REF] == 0);
        REQUIRE(listener.counts[CR_REF] == 1);
    }

    SECTION("listen for const&& events")
    {
        TestEvent_Listener<TestEvent1> listener;

        REQUIRE(listener.counts[VAL] == 0);
        REQUIRE(listener.counts[L_REF] == 0);
        REQUIRE(listener.counts[CL_REF] == 0);
        REQUIRE(listener.counts[R_REF] == 0);
        REQUIRE(listener.counts[CR_REF] == 0);

        TestEvent::Variant event = TestEvent1{};
        TestEvent::Dispatch(listener,std::move(event));

        REQUIRE(listener.counts[VAL] == 0);
        REQUIRE(listener.counts[L_REF] == 0);
        REQUIRE(listener.counts[CL_REF] == 0);
        REQUIRE(listener.counts[R_REF] == 1);
        REQUIRE(listener.counts[CR_REF] == 0);
    }
}

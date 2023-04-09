
#include <catch2/catch_all.hpp>

#include "RockHopper/Event/Event.hh"

#include <memory>
#include <unordered_map>
#include <type_traits>

/* ************************************************************************** */

struct TestEvent1 { std::unique_ptr<int> no_copy; };
struct TestEvent2 { std::unique_ptr<int> no_copy; };
using TestEvent = RockHopper::Event::EventSet<TestEvent1,TestEvent2>;

/* ************************************************************************** */

TEST_CASE("EventSet::Dispatch correctly dispatches an event to a listener")
{
    struct Listener : TestEvent::Listener
    {
        enum ArgumentQualifier
        {
            VAL, L_REF, CL_REF, R_REF, CR_REF,
        };
        std::unordered_map<ArgumentQualifier,size_t> counts {};

        using TestEvent::Listener::on_event;
        void on_event(TestEvent1 const&) override { counts[CL_REF] += 1; }
        void on_event(TestEvent1 &) override { counts[L_REF] += 1; }
        void on_event(TestEvent1 const&&) override { counts[CR_REF] += 1; }
        void on_event(TestEvent1 &&) override { counts[R_REF] += 1; }
    };
    Listener listener;

    REQUIRE(listener.counts[Listener::VAL] == 0);
    REQUIRE(listener.counts[Listener::L_REF] == 0);
    REQUIRE(listener.counts[Listener::CL_REF] == 0);
    REQUIRE(listener.counts[Listener::R_REF] == 0);
    REQUIRE(listener.counts[Listener::CR_REF] == 0);

    SECTION("listen for const& events")
    {
        TestEvent::Variant const event = TestEvent1{};
        TestEvent::Dispatch(listener,event);

        REQUIRE(listener.counts[Listener::VAL] == 0);
        REQUIRE(listener.counts[Listener::L_REF] == 0);
        REQUIRE(listener.counts[Listener::CL_REF] == 1);
        REQUIRE(listener.counts[Listener::R_REF] == 0);
        REQUIRE(listener.counts[Listener::CR_REF] == 0);
    }

    SECTION("listen for & events")
    {
        TestEvent::Variant event = TestEvent1{};
        TestEvent::Dispatch(listener,event);

        REQUIRE(listener.counts[Listener::VAL] == 0);
        REQUIRE(listener.counts[Listener::L_REF] == 1);
        REQUIRE(listener.counts[Listener::CL_REF] == 0);
        REQUIRE(listener.counts[Listener::R_REF] == 0);
        REQUIRE(listener.counts[Listener::CR_REF] == 0);
    }

    SECTION("listen for const&& events")
    {
        TestEvent::Variant const event = TestEvent1{};
        TestEvent::Dispatch(listener,std::move(event));

        REQUIRE(listener.counts[Listener::VAL] == 0);
        REQUIRE(listener.counts[Listener::L_REF] == 0);
        REQUIRE(listener.counts[Listener::CL_REF] == 0);
        REQUIRE(listener.counts[Listener::R_REF] == 0);
        REQUIRE(listener.counts[Listener::CR_REF] == 1);
    }

    SECTION("listen for const&& events")
    {
        TestEvent::Variant event = TestEvent1{};
        TestEvent::Dispatch(listener,std::move(event));

        REQUIRE(listener.counts[Listener::VAL] == 0);
        REQUIRE(listener.counts[Listener::L_REF] == 0);
        REQUIRE(listener.counts[Listener::CL_REF] == 0);
        REQUIRE(listener.counts[Listener::R_REF] == 1);
        REQUIRE(listener.counts[Listener::CR_REF] == 0);
    }
}

TEST_CASE("EventSet::MakeEvent correctly makes an instance of EventSet::Variant")
{
    auto event = TestEvent::MakeEvent<TestEvent1>();
    REQUIRE(std::is_same<TestEvent::Variant,std::remove_cv<decltype(event)>::type>::value);
}

TEST_CASE("EventSet::MakeListener correctly makes an event listener")
{
    size_t count = 0;

    auto listener = TestEvent::MakeListener<TestEvent1>([&]<typename E>(E&& event)
    {
        count += 1;
    });

    REQUIRE(count == 0);

    TestEvent::Variant const event = TestEvent::MakeEvent<TestEvent1>();
    TestEvent::Dispatch(listener,event);

    REQUIRE(count == 1);
}

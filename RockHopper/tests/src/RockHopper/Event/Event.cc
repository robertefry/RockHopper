
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
    enum class ArgumentQualifier
    {
        NONE, L_REF, CL_REF, R_REF, CR_REF,
    };

    constexpr auto RunTests = []<typename T_Listener>(T_Listener&& listener)
    {
        REQUIRE(listener.counts[ArgumentQualifier::NONE] == 0);
        REQUIRE(listener.counts[ArgumentQualifier::L_REF] == 0);
        REQUIRE(listener.counts[ArgumentQualifier::CL_REF] == 0);
        REQUIRE(listener.counts[ArgumentQualifier::R_REF] == 0);
        REQUIRE(listener.counts[ArgumentQualifier::CR_REF] == 0);

        SECTION("listen for const& variant events")
        {
            TestEvent::Variant const event = TestEvent1{};
            TestEvent::Dispatch(std::forward<T_Listener>(listener),event);

            REQUIRE(listener.counts[ArgumentQualifier::NONE] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::L_REF] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::CL_REF] == 1);
            REQUIRE(listener.counts[ArgumentQualifier::R_REF] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::CR_REF] == 0);
        }
        SECTION("listen for const& concrete events")
        {
            TestEvent1 const event = TestEvent1{};
            TestEvent::Dispatch(std::forward<T_Listener>(listener),event);

            REQUIRE(listener.counts[ArgumentQualifier::NONE] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::L_REF] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::CL_REF] == 1);
            REQUIRE(listener.counts[ArgumentQualifier::R_REF] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::CR_REF] == 0);
        }

        SECTION("listen for & variant events")
        {
            TestEvent::Variant event = TestEvent1{};
            TestEvent::Dispatch(std::forward<T_Listener>(listener),event);

            REQUIRE(listener.counts[ArgumentQualifier::NONE] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::L_REF] == 1);
            REQUIRE(listener.counts[ArgumentQualifier::CL_REF] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::R_REF] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::CR_REF] == 0);
        }
        SECTION("listen for & concrete events")
        {
            TestEvent1 event = TestEvent1{};
            TestEvent::Dispatch(std::forward<T_Listener>(listener),event);

            REQUIRE(listener.counts[ArgumentQualifier::NONE] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::L_REF] == 1);
            REQUIRE(listener.counts[ArgumentQualifier::CL_REF] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::R_REF] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::CR_REF] == 0);
        }

        SECTION("listen for const&& variant events")
        {
            TestEvent::Variant const event = TestEvent1{};
            TestEvent::Dispatch(std::forward<T_Listener>(listener),std::move(event));

            REQUIRE(listener.counts[ArgumentQualifier::NONE] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::L_REF] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::CL_REF] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::R_REF] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::CR_REF] == 1);
        }
        SECTION("listen for const&& concrete events")
        {
            TestEvent1 const event = TestEvent1{};
            TestEvent::Dispatch(std::forward<T_Listener>(listener),std::move(event));

            REQUIRE(listener.counts[ArgumentQualifier::NONE] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::L_REF] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::CL_REF] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::R_REF] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::CR_REF] == 1);
        }

        SECTION("listen for const&& variant events")
        {
            TestEvent::Variant event = TestEvent1{};
            TestEvent::Dispatch(std::forward<T_Listener>(listener),std::move(event));

            REQUIRE(listener.counts[ArgumentQualifier::NONE] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::L_REF] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::CL_REF] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::R_REF] == 1);
            REQUIRE(listener.counts[ArgumentQualifier::CR_REF] == 0);
        }
        SECTION("listen for const&& concrete events")
        {
            TestEvent1 event = TestEvent1{};
            TestEvent::Dispatch(std::forward<T_Listener>(listener),std::move(event));

            REQUIRE(listener.counts[ArgumentQualifier::NONE] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::L_REF] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::CL_REF] == 0);
            REQUIRE(listener.counts[ArgumentQualifier::R_REF] == 1);
            REQUIRE(listener.counts[ArgumentQualifier::CR_REF] == 0);
        }
    };

    SECTION("non-const listener")
    {
        /*
         * When using non-const listener, the non-const on_event functions
         * should be invoked. Thus, we use ArgumentQualifier::NONE for the const
         * qualified on_event functions to ensure they are not called.
         */
        struct Listener : TestEvent::Listener
        {
            std::unordered_map<ArgumentQualifier,size_t> mutable counts {};

            using TestEvent::Listener::on_event;

            void on_event(TestEvent1 const&) override { counts[ArgumentQualifier::CL_REF] += 1; }
            void on_event(TestEvent1 &) override { counts[ArgumentQualifier::L_REF] += 1; }
            void on_event(TestEvent1 const&&) override { counts[ArgumentQualifier::CR_REF] += 1; }
            void on_event(TestEvent1 &&) override { counts[ArgumentQualifier::R_REF] += 1; }

            /* using ArgumentQualifier::NONE */
            void on_event(TestEvent1 const&) const override { counts[ArgumentQualifier::NONE] += 1; }
            void on_event(TestEvent1 &) const override { counts[ArgumentQualifier::NONE] += 1; }
            void on_event(TestEvent1 const&&) const override { counts[ArgumentQualifier::NONE] += 1; }
            void on_event(TestEvent1 &&) const override { counts[ArgumentQualifier::NONE] += 1; }
        };
        Listener listener;

        RunTests(std::move(listener));
    }

    SECTION("const listener")
    {
        /*
         * When using const listener, the const on_event functions should be
         * invoked. Thus, we use ArgumentQualifier::NONE for the non-const
         * qualified on_event functions to ensure they are not called.
         */
        struct Listener : TestEvent::Listener
        {
            std::unordered_map<ArgumentQualifier,size_t> mutable counts {};

            using TestEvent::Listener::on_event;

            /* using ArgumentQualifier::NONE */
            void on_event(TestEvent1 const&) override { counts[ArgumentQualifier::NONE] += 1; }
            void on_event(TestEvent1 &) override { counts[ArgumentQualifier::NONE] += 1; }
            void on_event(TestEvent1 const&&) override { counts[ArgumentQualifier::NONE] += 1; }
            void on_event(TestEvent1 &&) override { counts[ArgumentQualifier::NONE] += 1; }

            void on_event(TestEvent1 const&) const override { counts[ArgumentQualifier::CL_REF] += 1; }
            void on_event(TestEvent1 &) const override { counts[ArgumentQualifier::L_REF] += 1; }
            void on_event(TestEvent1 const&&) const override { counts[ArgumentQualifier::CR_REF] += 1; }
            void on_event(TestEvent1 &&) const override { counts[ArgumentQualifier::R_REF] += 1; }
        };
        Listener const listener;

        RunTests(std::move(listener));
    }
}


// #define ROCKHOPPER_LOG_EVENT_DISPATCH
#include "RockHopper/Event/Event.hh"
#include "RockHopper/Event/EventHandler.hh"
#include "RockHopper/Event/EventListener.hh"
using namespace RockHopper::Event;

#include <gtest/gtest.h>
#include <string>
#include <iostream>

/* ************************************************************************** */

struct TestEventCategory;
struct TestEvent1;
struct TestEvent2;

using TestEventSet = EventSet<TestEventCategory,
    TestEvent1, TestEvent2 >;

struct TestEventCategory
    : EventCategory<TestEventSet>
{
};

struct TestEvent1
    : EventListenable<TestEventCategory,TestEvent1>
{
};

struct TestEvent2
    : EventListenable<TestEventCategory,TestEvent2>
{
};

/* ************************************************************************** */

template <typename T_EventDispatch>
struct TestEventHandler
    : public EventHandler<TestEventCategory,T_EventDispatch>
{
    using EventHandler<TestEventCategory,T_EventDispatch>::dispatch_event;
};

TEST(EVENTS,DISPATCH_SEQUENTIAL)
{
    TestEventHandler<Dispatch::SequentialDispatch> event_handler;

    struct TestEvent_Listener
        : public TestEventCategory::ListenerType
    {
        virtual void on_event_category(TestEventCategory const&) override
        {
            // ROCKHOPPER_LOG_INFO(__PRETTY_FUNCTION__);
            m_CallCount_Category += 1;
        }
        virtual void on_event(TestEvent1 const&) override
        {
            // ROCKHOPPER_LOG_INFO(__PRETTY_FUNCTION__);
            m_CallCount_Event1 += 1;
        }
        virtual void on_event(TestEvent2 const&) override
        {
            // ROCKHOPPER_LOG_INFO(__PRETTY_FUNCTION__);
            m_CallCount_Event2 += 1;
        }
    public:
        unsigned int m_CallCount_Event1 = 0;
        unsigned int m_CallCount_Event2 = 0;
        unsigned int m_CallCount_Category = 0;
    };

    auto listener = std::make_shared<TestEvent_Listener>();
    event_handler.insert_event_listener(listener);

    EXPECT_EQ(listener->m_CallCount_Event1,0);
    EXPECT_EQ(listener->m_CallCount_Event2,0);
    EXPECT_EQ(listener->m_CallCount_Category,0);

    // ROCKHOPPER_LOG_DEBUG("EventHandler<TestEventCategory>::dispatch_event(TestEvent1{})");
    event_handler.dispatch_event(TestEvent1{});

    EXPECT_EQ(listener->m_CallCount_Event1,1);
    EXPECT_EQ(listener->m_CallCount_Event2,0);
    EXPECT_EQ(listener->m_CallCount_Category,1);

    // ROCKHOPPER_LOG_DEBUG("EventHandler<TestEventCategory>::dispatch_event(TestEvent2{})");
    event_handler.dispatch_event(TestEvent2{});

    EXPECT_EQ(listener->m_CallCount_Event1,1);
    EXPECT_EQ(listener->m_CallCount_Event2,1);
    EXPECT_EQ(listener->m_CallCount_Category,2);
}

TEST(EVENTS,EVENT_LISTENER)
{
    TestEventHandler<Dispatch::SequentialDispatch> event_handler;

    unsigned int call_count = 0;

    auto listener = MakeListener<TestEvent1>([&](TestEvent1 const& event)
    {
        call_count += 1;
    });
    event_handler.insert_event_listener(listener);

    EXPECT_EQ(call_count,0);

    event_handler.dispatch_event(TestEvent1{});

    EXPECT_EQ(call_count,1);

    event_handler.dispatch_event(TestEvent2{});

    EXPECT_EQ(call_count,1);
}

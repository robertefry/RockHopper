
# The Idea

The idea behind this somewhat convoluted event system is threefold;

1. That each subsystem should be aware only of the events related to that subsystem. <br> e.g; a keyboard should be aware only of keyboard events
2. The dispatch mechanism for each event should not be aware of which event it's dispatching. <br> i.e; there should be a parent type which every event in a subsystem implements from.
3. The event system should require no more overhead than dumb enumeration.

Thus, the chosen way to build this system is a variation of the visitor pattern.

# Events

Each set of events for a subsystem are related by defining an event category. To make the event system aware of all possible events, we must also define an event set which lists all events of the same category in a template parameter list.

```cpp
struct TestEventCategory;
struct TestEvent1;
struct TestEvent2;

using TestEventSet = EventSet<TestEventCategory,
    TestEvent1, TestEvent2 >;
```

We then define each event in the category.

```cpp
struct TestEventCategory
    : EventCategory<TestEventSet>
{
};
```
```cpp
struct TestEvent1
    : EventListenable<TestEventCategory,TestEvent1>
{
    // extends TestEventCategory by deferred CRTP
};

struct TestEvent2
    : EventListenable<TestEventCategory,TestEvent2>
{
    // extends TestEventCategory by deferred CRTP
};
```

# Event Listeners

Each EventListenable, EventCategory, and EventSet is equipped with an event listener, ListenerType, which defines a set of virtual on_event functions for every event in the category. This serves as the parent type for any event listener.

```cpp
struct My_TestEvent1_Listener
    : public TestEvent1::ListenerType
{
    virtual void on_event(TestEvent1 const& event) override
    {
        // handle the TestEvent1 event
    }
};
```

When implementing from an event listener, only the on_event function for that event will be overridable. However, when implementing from the event set or event category, the on_event function for every event in the category will be overridable. Furthermore, an on_event_category function is be overridable, which is called for every event dispatched in the category.

```cpp
struct My_TestEventCategory_Listener
    : public TestEventCategory::ListenerType
{
    virtual void on_event_category(TestEventCategory const& event) override
    {
        // handle any event in the TestEventCategory category
    }
    virtual void on_event(TestEvent1 const& event) override
    {
        // handle a TestEvent1 event
    }
    virtual void on_event(TestEvent2 const& event) override
    {
        // handle a TestEvent2 event
    }
};
```

## The EventListener Class

The EventListener class will adapt any std::function into a listener, provided it matches the fingerprint for an on_event function.

```cpp
auto listener = EventListener<TestEvent1>{[](TestEvent1 const& event)
{
    // handle a TestEvent1 event
}};
// listener is of type EventListener<TestEvent1>
```

The MakeListener global function is available to aid making shared_ptr listeners.

```cpp
auto listener = MakeListener([](TestEvent1 const& event)
{
    // handle a TestEvent1 event
});
// listener is of type std::shared_ptr<EventListener<TestEvent1>>
```

# Dispatching Events

An EventHandler instance is used to dispatch events to every listener it has a handle for.

```cpp
EventHandler<TestEventCategory> event_handler;
```

We supply the event handler with listeners, using std::shared_ptr types.

```cpp
auto listener = std::make_shared<My_TestEvent1_Listener>();
event_listener.insert_event_listener(listener);
```

An event is dispatched to every listener in an event handler, simply by calling the event handlers dispatch function.

```cpp
TestEvent1 event;
event_handler.dispatch_event(event);
```

If a listener does not listen for an event in the category, it is simply ignored by that listener.

## Event Dispatchers

Optionally, the EventHandler will take a second template argument specifying the event dispatch mechanism. Available dispatch mechanisms are sequential, parallel, and pooled dispatchers. The default mechanism is sequential dispatch.

```cpp
using DispatchMechanism = ParallelDispatch<TestEventCategory>;
EventHandler<TestEventCategory,DispatchMechanism> event_handler;
```

The dispatcher supplies a service function, which defines the dispatching mechanism.

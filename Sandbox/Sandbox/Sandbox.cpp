
#include "Sandbox.h"

#include "RockHopper/Logging/Logger.h"
#include "RockHopper/Event/EventHandler.h"

#include <iostream>

Sandbox::Sandbox()
{
}

Sandbox::~Sandbox()
{
}

void Sandbox::run()
{
    // define an event
    struct HelloWorldEvent {
        void sayHelloWorld() const { ROCKHOPPER_LOG_INFO("Hello, World!"); }
    };

    // create an event handler
    auto sandboxHelloWorldEventHandler = RockHopper::EventHandler<HelloWorldEvent>{};

    // register an event callback function
    sandboxHelloWorldEventHandler.registerEventCallback([](const HelloWorldEvent& event) {
        ROCKHOPPER_LOG_INFO("Handling a HelloWorld event.");
        event.sayHelloWorld();
    });

    // send a new event to the event handler
    sandboxHelloWorldEventHandler.handleEvent(HelloWorldEvent{});
}

#pragma once

#include <functional>
#include <iostream>
#include <memory>

#include "../game/event.h"

namespace SailGame { namespace Common {

using Game::Event;

class EventListener {
public:
    EventListener(const std::function<void(const std::shared_ptr<Event> &)> &callback)
        : OnEventHappens(callback) {}

protected:
    std::function<void(const std::shared_ptr<Event> &)> OnEventHappens;
};

class NetworkEventListener : public EventListener {
public:
    NetworkEventListener(const std::function<void(const std::shared_ptr<Event> &)> &callback)
        : EventListener(callback) {}
    
    void operator()();
};

class UserInputEventListener : public EventListener {
public:
    UserInputEventListener(const std::function<void(const std::shared_ptr<Event> &)> &callback)
        : EventListener(callback) {}

    void operator()();
};

class TimerEventListener : public EventListener {
public:
    TimerEventListener(const std::function<void(const std::shared_ptr<Event> &)> &callback)
        : EventListener(callback) {}

    void operator()();
};

}}
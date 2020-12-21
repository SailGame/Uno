#pragma once

#include <functional>
#include <iostream>
#include <memory>

#include "../game/event.h"
#include "../network/client.h"

namespace SailGame { namespace Common {

using Game::Event;
using Network::Client;

class EventListener {
public:
    EventListener(const std::function<void(const std::shared_ptr<Event> &)> &callback)
        : OnEventHappens(callback) {}

protected:
    std::function<void(const std::shared_ptr<Event> &)> OnEventHappens;
};

class NetworkEventListener : public EventListener {
public:
    NetworkEventListener(const std::function<void(const std::shared_ptr<Event> &)> &callback,
        const std::string &serverAddr)
        : EventListener(callback), mClient(std::make_unique<Client>(serverAddr)) {}
    
    void operator()();

private:
    std::unique_ptr<Client> mClient;
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
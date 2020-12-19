#pragma once

#include <iostream>
#include <memory>

#include "state.h"

namespace SailGame { namespace Common {

class Event {
public:
    virtual void Process(std::shared_ptr<Game::State> &state) = 0;
};

class NetPacketEvent : public Event {
};

class UserInputEvent : public Event {
public:
    UserInputEvent(char c) : mCharInputted(c) {}

    virtual void Process(std::shared_ptr<Game::State> &state) override;

private:
    char mCharInputted;
};

class TimerEvent : public Event {
};

}}
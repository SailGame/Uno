#pragma once

#include <iostream>
#include <memory>

#include "state.h"

namespace SailGame { namespace Game {

class Event {
public:
    virtual void Process(std::shared_ptr<State> &state) = 0;
};

class NetworkEvent : public Event {
};

class UserInputEvent : public Event {
public:
    UserInputEvent(char c) : mCharInputted(c) {}

    virtual void Process(std::shared_ptr<State> &state) override;

private:
    char mCharInputted;
};

class TimerEvent : public Event {
};

}}
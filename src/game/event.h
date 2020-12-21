#pragma once

#include <iostream>
#include <memory>

#include "hello.pb.h"
#include "state.h"

namespace SailGame { namespace Game {

using Uno::NotifyMsg;
using Uno::Draw;

class Event {
public:
    virtual void Process(std::shared_ptr<State> &state) = 0;
};

class NetworkEvent : public Event {
public:
    static std::shared_ptr<NetworkEvent> Create(const NotifyMsg &msg);

    virtual void Process(std::shared_ptr<State> &state) override = 0;
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

class DrawNetworkEvent : public NetworkEvent {
public:
    DrawNetworkEvent(const Draw &draw) : mDraw(draw) {}

    virtual void Process(std::shared_ptr<State> &state) override;

private:
    Draw mDraw;
};
}}
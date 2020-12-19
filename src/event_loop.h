#pragma once

#include <memory>
#include <queue>

#include "state_machine.h"

namespace SailGame { namespace Common {

class EventLoop {
public:
    EventLoop() {
        /// XXX: use factory
        mStateMachine = std::make_shared<StateMachine>();
    }

    void StartLoop();

    void AppendEvent(const std::shared_ptr<Event> &event);

private:
    std::queue<std::shared_ptr<Event>> mEventQueue;
    std::shared_ptr<StateMachine> mStateMachine;
};
}}
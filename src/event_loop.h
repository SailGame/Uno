#pragma once

#include <memory>
#include <queue>

#include "event.h"

namespace SailGame { namespace Common {

class EventLoop {
public:
    EventLoop() = default;

    void StartLoop();

    void AppendEvent(const std::shared_ptr<Event> &event);

private:
    std::queue<std::shared_ptr<Event>> mEventQueue;
};
}}
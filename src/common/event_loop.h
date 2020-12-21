#pragma once

#include <functional>
#include <memory>
#include <queue>

#include "../game/event.h"

namespace SailGame { namespace Common {

using Game::Event;

class EventLoop {
public:
    EventLoop() {}

    void StartLoop();

    void AppendEvent(const std::shared_ptr<Event> &event);

    void RegisterEventProcessedCallback(
        const std::function<void(const std::shared_ptr<Event> &)> &callback);
    
private:
    std::function<void(const std::shared_ptr<Event> &)> OnEventProcessed;

private:
    std::queue<std::shared_ptr<Event>> mEventQueue;
    std::mutex mMutex;
};
}}
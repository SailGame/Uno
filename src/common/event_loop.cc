#include "event_loop.h"

namespace SailGame { namespace Common {

void EventLoop::StartLoop()
{
    while (true) {
        if (!mEventQueue.empty()) {
            OnEventProcessed(mEventQueue.front());
            mEventQueue.pop();
        }
    }
}

void EventLoop::AppendEvent(const std::shared_ptr<Event> &event)
{
    // three threads may invoke this at the same time
    std::lock_guard<std::mutex> lock(mMutex);
    mEventQueue.push(event);
}

void EventLoop::RegisterEventProcessedCallback(
        const std::function<void(const std::shared_ptr<Event> &)> &callback)
{
    OnEventProcessed = callback;
}

}}
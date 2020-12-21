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
    mEventQueue.push(event);
}

void EventLoop::RegisterEventProcessedCallback(
        const std::function<void(const std::shared_ptr<Event> &)> &callback)
{
    OnEventProcessed = callback;
}

}}
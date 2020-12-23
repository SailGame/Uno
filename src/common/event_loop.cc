#include "event_loop.h"

namespace SailGame { namespace Common {

void EventLoop::StartLoop()
{
    while (true) {
        if (!mEventQueue.empty()) {
            std::cout << "[EventLoop] Process Event: " << int(mEventQueue.front()->mType) << std::endl;
            OnEventProcessed(mEventQueue.front());
            mEventQueue.pop();
        }
    }
}

void EventLoop::AppendEvent(const std::shared_ptr<Event> &event)
{
    // three threads may invoke this at the same time
    std::lock_guard<std::mutex> lock(mMutex);
    std::cout << "[EventLoop] Append Event: " << int(event->mType) << std::endl;
    mEventQueue.push(event);
}

void EventLoop::RegisterEventProcessedCallback(
        const std::function<void(const std::shared_ptr<Event> &)> &callback)
{
    OnEventProcessed = callback;
}

}}
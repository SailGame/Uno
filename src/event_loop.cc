#include "event_loop.h"

namespace SailGame { namespace Common {

void EventLoop::StartLoop()
{
    while (true) {
        if (!mEventQueue.empty()) {
            mEventQueue.front()->Process();
            mEventQueue.pop();
        }
    }
}

void EventLoop::AppendEvent(const std::shared_ptr<Event> &event)
{
    mEventQueue.push(event);
}
}}
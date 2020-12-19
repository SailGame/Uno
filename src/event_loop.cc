#include "event_loop.h"

namespace SailGame { namespace Common {

void EventLoop::StartLoop()
{
    while (true) {
        if (!mEventQueue.empty()) {
            mStateMachine->Transition(mEventQueue.front());
            mEventQueue.pop();
        }
    }
}

void EventLoop::AppendEvent(const std::shared_ptr<Event> &event)
{
    mEventQueue.push(event);
}
}}
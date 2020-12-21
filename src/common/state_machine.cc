#include "state_machine.h"

namespace SailGame { namespace Common {

void StateMachine::Transition(const std::shared_ptr<Event> &event)
{
    event->Process(mState);
}
}}
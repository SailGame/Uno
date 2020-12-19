#pragma once

#include "event.h"

namespace SailGame { namespace Common {

class StateMachine {
public:
    void Transition(const std::shared_ptr<Event> &event);

private:
    std::shared_ptr<Game::State> mState;
};
}}
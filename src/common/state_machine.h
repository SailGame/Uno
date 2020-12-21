#pragma once

#include "../game/event.h"

namespace SailGame { namespace Common {

using Game::Event;
using Game::State;

class StateMachine {
public:
    void Transition(const std::shared_ptr<Event> &event);

private:
    std::shared_ptr<State> mState;
};
}}
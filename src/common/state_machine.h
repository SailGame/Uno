#pragma once

#include "../game/event.h"

namespace SailGame { namespace Common {

using Game::Event;
using Game::State;

template<typename StateT>
class StateMachine {
public:
    static_assert(std::is_same_v<StateT, State>);
    
    StateMachine() : mState(std::make_shared<StateT>()) {}

    void Transition(const std::shared_ptr<Event> &event) {
        event->Process(mState);
    }

private:
    std::shared_ptr<StateT> mState;
};
}}
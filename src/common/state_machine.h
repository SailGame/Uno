#pragma once

#include <variant>
#include <type_traits>

#include "../game/event.h"
#include "types.h"

namespace SailGame { namespace Common {

using Game::Event;
using Game::State;
using Uno::UserOperation;
using Uno::NotifyMsg;

template<typename StateT>
class StateMachine {
public:
    static_assert(std::is_same_v<StateT, State>);

    StateMachine() : mState(std::make_shared<StateT>()) {}

    MsgTypePtr Transition(const std::shared_ptr<Event> &event) {
        return event->Process(mState);
    }

private:
    std::shared_ptr<StateT> mState;
};
}}
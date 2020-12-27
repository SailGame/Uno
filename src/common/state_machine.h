#pragma once

#include <memory>

#include "types.h"

namespace SailGame { namespace Common {

template<typename StateT>
class StateMachine {
public:
    StateMachine() = default;

    template<typename MsgT>
    ProviderMsgPtrs Transition(const MsgT &event) {
        throw std::runtime_error("Unsupported msg type");
    }

private:
    StateT mState;
};
}}
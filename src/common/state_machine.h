#pragma once

#include <memory>

#include "types.h"

namespace SailGame { namespace Common {

template<typename StateT>
class StateMachine {
public:
    StateMachine() : mState(std::make_unique<StateT>()) {}

    template<typename MsgT>
    ProviderMsgPtrs Transition(const MsgT &event) {
        assert(false);
    }

private:
    std::unique_ptr<StateT> mState;
};
}}
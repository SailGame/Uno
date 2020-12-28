#pragma once

#include <memory>

#include "types.h"

namespace SailGame { namespace Common {

template<typename StateT>
class StateMachine {
public:
    static StateMachine &Create() {
        static StateMachine staticMachine;
        return staticMachine;
    }

    template<typename MsgT>
    ProviderMsgPtrs Transition(const MsgT &event) {
        throw std::runtime_error("Unsupported msg type");
    }

private:
    StateMachine() = default;

private:
    StateT mState;
};
}}
#pragma once

#include <memory>

#include "types.h"

namespace SailGame { namespace Common {

template<typename StateT>
class StateMachine {
public:
    template<typename MsgT>
    ProviderMsgPtrs Transition(const MsgT &event);

private:
    std::unique_ptr<StateT> mState;
};
}}
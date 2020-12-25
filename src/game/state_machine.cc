#include "state.h"
#include "../common/state_machine.h"

#include <core/provider.pb.h>

namespace SailGame { namespace Common {

using Common::StateMachine;
using Common::ProviderMsgPtrs;
using Core::ProviderMsg;

template<>
template<>
ProviderMsgPtrs StateMachine<Game::State>::Transition<ProviderMsg>(const ProviderMsg &msg)
{
    return {};
}
}}
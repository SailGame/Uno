#pragma once

#include <memory>
#include <sailgame/common/state_machine.h>
#include <sailgame/common/types.h>
#include <sailgame_pb/core/provider.pb.h>

#include "state.h"

namespace SailGame { namespace Uno {

using ::Core::CloseGameArgs;
using ::Core::ProviderMsg;
using ::Core::QueryStateArgs;
using ::Core::RegisterRet;
using ::Core::StartGameArgs;
using ::Core::UserOperationArgs;
using ::Uno::Draw;
using ::Uno::Skip;
using ::Uno::Play;
using ::Uno::UserOperation;
using Common::IStateMachine;
using Common::ProviderMsgs;

class StateMachine : public IStateMachine {
public:
    StateMachine() = default;

    static std::shared_ptr<IStateMachine> Create() {
        return std::make_shared<StateMachine>();
    }

    const GlobalState &GetState() { return mState; }

protected:
    virtual ProviderMsgs Transition(const RegisterRet &msg) override;

    virtual ProviderMsgs Transition(const StartGameArgs &msg) override;

    virtual ProviderMsgs Transition(const CloseGameArgs &msg) override;

    virtual ProviderMsgs Transition(const QueryStateArgs &msg) override;

    virtual ProviderMsgs Transition(const UserOperationArgs &msg) override;

private:
    ProviderMsgs Transition(const UserOperation &msg);
    
    ProviderMsgs Transition(const Draw &msg);

    ProviderMsgs Transition(const Skip &msg);

    ProviderMsgs Transition(const Play &msg);

private:
    GlobalState mState;
};
}}
#include <spdlog/spdlog.h>

#include "state.h"
#include "msg_builder.h"
#include "../common/state_machine.h"
#include "../common/util.h"

#include <core/provider.pb.h>
#include <core/error.pb.h>
#include <uno/uno.pb.h>

namespace SailGame { namespace Common {

using Game::GlobalState;
using Game::MsgBuilder;
using Core::ProviderMsg;
using Core::RegisterRet;
using Core::StartGameArgs;
using Core::CloseGameArgs;
using Core::QueryStateArgs;
using Core::UserOperationArgs;
using Core::NotifyMsgArgs;
using ::Uno::StartGameSettings;
using ::Uno::UserOperation;
using ::Uno::NotifyMsg;
using ::Uno::Draw;
using ::Uno::Skip;
using ::Uno::Play;
using ::Uno::Uno;
using ::Uno::Catch;
using ::Uno::Doubt;
using ::Uno::GameStart;
using ::Uno::Exit;

#define TransitionFor(MsgT) \
    template<> \
    template<> \
    ProviderMsgPtrs StateMachine<GlobalState>::Transition<MsgT>(const MsgT &msg)

#define ForwardDeclaration(MsgT) \
    TransitionFor(MsgT);

TransitionFor(ProviderMsg);
TransitionFor(RegisterRet);
TransitionFor(StartGameArgs);
// TransitionFor(CloseGameArgs);
// TransitionFor(QueryStateArgs);
// TransitionFor(UserOperationArgs);

TransitionFor(ProviderMsg)
{
    switch (msg.Msg_case()) {
        case ProviderMsg::MsgCase::kRegisterRet:
            return Transition<RegisterRet>(msg.registerret());
        case ProviderMsg::MsgCase::kStartGameArgs:
            return Transition<StartGameArgs>(msg.startgameargs());
        case ProviderMsg::MsgCase::kCloseGameArgs:
            return Transition<CloseGameArgs>(msg.closegameargs());
        case ProviderMsg::MsgCase::kQueryStateArgs:
            return Transition<QueryStateArgs>(msg.querystateargs());
        case ProviderMsg::MsgCase::kUserOperationArgs:
            return Transition<UserOperationArgs>(msg.useroperationargs());
    }
    return {};
}

TransitionFor(RegisterRet)
{
    if (msg.err() != Core::ErrorNumber::OK) {
        spdlog::error("Register Failure");
    }
    return {};
}

TransitionFor(StartGameArgs)
{
    auto [userIdToInitHandcards, flippedCard, firstPlayer] = mState->NewGame(
        msg.roomid(), Util::ConvertGrpcRepeatedFieldToVector(msg.userid()),
        Util::UnpackGrpcAnyTo<StartGameSettings>(msg.custom()));

    ProviderMsgPtrs msgs;
    for (const auto &entry : userIdToInitHandcards) {
        msgs.push_back(
            MsgBuilder::CreateNotifyMsgArgs(0, Core::ErrorNumber::OK, msg.roomid(), entry.first,
                MsgBuilder::CreateGameStart(entry.second, flippedCard, firstPlayer))
        );
    }
    return msgs;
}
}}
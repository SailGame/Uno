#include <spdlog/spdlog.h>

#include <sailgame_pb/core/provider.pb.h>
#include <sailgame_pb/core/error.pb.h>
#include <sailgame_pb/uno/uno.pb.h>

#include <sailgame/common/state_machine.h>
#include <sailgame/common/util.h>
#include <sailgame/common/provider_msg_builder.h>
#include <sailgame/uno/msg_builder.h>

#include "state_machine.h"
#include "logger.h"

namespace SailGame { namespace Uno {

using Common::ProviderMsgBuilder;
using ::Uno::Catch;
using ::Uno::Doubt;
using ::Uno::Exit;
using ::Uno::GameStart;
using ::Uno::NotifyMsg;
using ::Uno::StartGameSettings;
using ::Uno::Uno;

ProviderMsgs StateMachine::Transition(const RegisterRet &msg)
{
    if (msg.err() != Core::ErrorNumber::OK) {
        spdlog::error("Register Failure");
    }
    return {};
}

ProviderMsgs StateMachine::Transition(const StartGameArgs &msg)
{
    spdlog::info("Game Start!");
    auto [userIdToInitHandcards, flippedCard, firstPlayer] = mState.NewGame(
        msg.roomid(), Util::ConvertGrpcRepeatedFieldToVector(msg.userid()),
        Util::UnpackGrpcAnyTo<StartGameSettings>(msg.custom()));

    ProviderMsgs msgs;
    for (const auto &entry : userIdToInitHandcards) {
        auto gameStart = MsgBuilder::CreateGameStart(
            entry.second, flippedCard, firstPlayer);
        auto msgToSend = ProviderMsgBuilder::CreateNotifyMsgArgs(
            0, Core::ErrorNumber::OK, msg.roomid(), entry.first, gameStart);
        Logger::Log(msgToSend);
        msgs.push_back(msgToSend);
    }
    return msgs;
}

ProviderMsgs StateMachine::Transition(const QueryStateArgs &msg) { return {}; }

ProviderMsgs StateMachine::Transition(const UserOperationArgs &msg)
{
    mState.mCurrentRoomId = msg.roomid();
    mState.mCurrentUserId = msg.userid();
    return Transition(Util::UnpackGrpcAnyTo<UserOperation>(msg.custom()));
}

ProviderMsgs StateMachine::Transition(const UserOperation &msg)
{
    switch (msg.Operation_case()) {
        case UserOperation::OperationCase::kDraw:
            return Transition(msg.draw());
        case UserOperation::OperationCase::kSkip:
            return Transition(msg.skip());
        case UserOperation::OperationCase::kPlay:
            return Transition(msg.play());
            /// TODO: handle other operations
    }
    throw std::runtime_error("Unsupported msg type");
    return {};
}

ProviderMsgs StateMachine::Transition(const Draw &msg)
{
    Logger::Log(msg);
    auto roomId = mState.mCurrentRoomId;
    auto userId = mState.mCurrentUserId;
    auto &gameState = mState.mRoomIdToGameState.at(roomId);
    auto cards = gameState.mDeck.Draw(msg.number());
    gameState.mUserIdToPlayerState.at(userId).mHandcards.Draw(cards);

    // broadcast msg doesn't need to include userId of the player who sent UserOperation.
    // because in client side, it can be deduced from `mCurrentPlayer` in state machine
    ProviderMsgs msgs;
    msgs.push_back(
        ProviderMsgBuilder::CreateNotifyMsgArgs(
            0, Core::ErrorNumber::OK, roomId, 0,
            MsgBuilder::CreateDraw(msg)));
    msgs.push_back(
        ProviderMsgBuilder::CreateNotifyMsgArgs(
            0, Core::ErrorNumber::OK, roomId, userId,
            MsgBuilder::CreateDrawRsp(cards)));
    return msgs;
}

ProviderMsgs StateMachine::Transition(const Skip &msg)
{
    Logger::Log(msg);
    auto roomId = mState.mCurrentRoomId;
    auto userId = mState.mCurrentUserId;

    ProviderMsgs msgs;
    msgs.push_back(
        ProviderMsgBuilder::CreateNotifyMsgArgs(
            0, Core::ErrorNumber::OK, roomId, 0,
            MsgBuilder::CreateSkip(msg)));
    return msgs;
}

ProviderMsgs StateMachine::Transition(const Play &msg)
{
    Logger::Log(msg);
    auto roomId = mState.mCurrentRoomId;
    auto userId = mState.mCurrentUserId;
    auto &gameState = mState.mRoomIdToGameState.at(roomId);
    auto card = Card{msg.card()};
    gameState.mDiscardPile.Add(card);
    gameState.mUserIdToPlayerState.at(userId).mHandcards.Erase(card);

    ProviderMsgs msgs;
    msgs.push_back(
        ProviderMsgBuilder::CreateNotifyMsgArgs(
            0, Core::ErrorNumber::OK, roomId, 0,
            MsgBuilder::CreatePlay(msg)));

    if (gameState.mUserIdToPlayerState.at(userId).mHandcards.Number() == 0) {
        // game ends
        msgs.push_back(
            ProviderMsgBuilder::CreateCloseGameArgs(0, roomId));
        mState.mRoomIdToGameState.erase(roomId);
        spdlog::info("GameOver!");
    }
    return msgs;
}
}}
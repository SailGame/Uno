#pragma once

#include <string>
#include <spdlog/spdlog.h>
#include <sailgame/uno/card.h>
#include <sailgame/common/types.h>
#include <sailgame_pb/uno/uno.pb.h>
#include <sailgame_pb/core/types.pb.h>

#include "state.h"

namespace SailGame { namespace Uno {

using namespace ::Uno;
using Common::ProviderMsgs;
using ::Core::NotifyMsgArgs;
using ::Core::ProviderMsg;

class Logger {
public:
    static void Log(const ProviderMsg &msg_) {
        assert(msg_.has_notifymsgargs());
        auto msg = msg_.notifymsgargs();
        spdlog::info("[NotifyMsgArgs] err: {}, roomId: {}, userId: {}",
                     msg.err(), msg.roomid(), msg.userid());
    }

    static void Log(const GameStart &msg) {
        spdlog::info("[GameStart] cardsize: {}, flippedCard: {}, firstPlayer: {}",
            msg.inithandcards_size(), 
            SailGame::Uno::Card{msg.flippedcard()}.ToString(),
            msg.firstplayer());
    }

    static void Log(const Draw &msg) {
        spdlog::info("[Draw] number: {}", msg.number());
    }

    static void Log(const Skip &msg) {
        spdlog::info("[Skip]");
    }

    static void Log(const Play &msg) {
        spdlog::info("[Play] card: {}, nextColor: {}", 
            SailGame::Uno::Card{msg.card()}.ToString(),
            msg.nextcolor());
    }

    static void Log(const DrawRsp &msg) {
        spdlog::info("[DrawRsp] cardsize: {}", msg.cards_size());
    }

    // static void LogState(const WholeState &state) {
    //     const auto &gameState = state.mGameState;
    //     const auto &selfState = state.mSelfState;
    //     const auto &playerStates = state.mPlayerStates;
    //     spdlog::info(
    //         "[State] \nGameState: { "
    //         "playerNum: " + std::to_string(gameState.mPlayerNum) + "\t"
    //         "selfIndex: " + std::to_string(gameState.mSelfPlayerIndex) + "\t"
    //         "curPlayer: " + std::to_string(gameState.mCurrentPlayer) + "\t"
    //         "clockwise: " + std::to_string(gameState.mIsInClockwise) + "\t"
    //         "lastPlay : " + gameState.mLastPlayedCard.ToString() + " }\n"
    //         // "SelfState: {}\n"
    //         // "PlayerStates: {}\n"
    //     );
    // }
};
}}

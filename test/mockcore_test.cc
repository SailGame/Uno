#include <gtest/gtest.h>
#include <grpcpp/test/mock_stream.h>

#include <sailgame_pb/core/core_mock.grpc.pb.h>
#include <sailgame_pb/core/provider.pb.h>
#include <sailgame_pb/core/error.pb.h>
#include <sailgame_pb/uno/uno.pb.h>

#include <sailgame/uno/msg_builder.h>
#include <sailgame/common/game_manager.h>
#include <sailgame/common/util.h>

#include "../src/state.h"

namespace SailGame { namespace Test {

using namespace testing;
using Common::EventLoop;
using Common::GameManager;
using Common::NetworkInterface;
using Common::StateMachine;
using Common::Util;
using Core::ErrorNumber;
using Core::MockGameCoreStub;
using Core::ProviderMsg;
using Game::GlobalState;
using Game::MsgBuilder;
using Game::Card;
using grpc::testing::MockClientReaderWriter;
using ::Uno::GameStart;
using ::Uno::NotifyMsg;
using ::Uno::CardColor;
using ::Uno::CardText;

MATCHER_P3(NotifyMsgArgsMatcher, err, roomId, userId, "") {
    return arg.has_notifymsgargs() && arg.notifymsgargs().err() == err &&
        arg.notifymsgargs().roomid() == roomId &&
        arg.notifymsgargs().userid() == userId &&
        arg.notifymsgargs().custom().template Is<NotifyMsg>();
}

MATCHER(GameStartMatcher, "") {
    auto msg = Util::UnpackGrpcAnyTo<NotifyMsg>(arg.notifymsgargs().custom());
    return msg.has_gamestart();
}

MATCHER_P(DrawMatcher, number, "") {
    auto msg = Util::UnpackGrpcAnyTo<NotifyMsg>(arg.notifymsgargs().custom());
    return msg.has_draw() && msg.draw().number() == number;
}

MATCHER_P(DrawRspMatcher, number, "") {
    auto msg = Util::UnpackGrpcAnyTo<NotifyMsg>(arg.notifymsgargs().custom());
    return msg.has_drawrsp() && msg.drawrsp().cards_size() == number;
}

MATCHER(SkipMatcher, "") {
    auto msg = Util::UnpackGrpcAnyTo<NotifyMsg>(arg.notifymsgargs().custom());
    return msg.has_skip();
}

MATCHER_P2(PlayMatcher, card, color, "") {
    auto msg = Util::UnpackGrpcAnyTo<NotifyMsg>(arg.notifymsgargs().custom());
    return msg.has_play() && 
        Card{msg.play().card()} == card && msg.play().nextcolor() == color;
}

class MockCoreFixture : public ::testing::Test {
public:
    MockCoreFixture()
        : mMockStream(new MockClientReaderWriter<ProviderMsg, ProviderMsg>()),
        mMockStub(std::make_shared<MockGameCoreStub>()),
        mStateMachine(std::make_shared<StateMachine<GlobalState>>()),
        mNetworkInterface(NetworkInterface<true>::Create(mMockStub)),
        mGameManager(EventLoop::Create(), mStateMachine, mNetworkInterface)
    {}

    void SetUp() {
        spdlog::set_level(spdlog::level::err);
        EXPECT_CALL(*mMockStub, ProviderRaw(_))
            .Times(AtLeast(1)).WillOnce(Return(mMockStream));
        mThread = std::make_unique<std::thread>([&] {
            mGameManager.Start();
        });

        while (!mNetworkInterface->IsConnected()) {}
    }

    void TearDown() {
        mGameManager.Stop();
        mThread->join();
    }

    void ProcessMsgFromCore(const ProviderMsg &msg) {
        EXPECT_CALL(*mMockStream, Read(_)).Times(1).WillOnce(
            DoAll(SetArgPointee<0>(msg), Return(true)));
        mNetworkInterface->OnEventHappens(mNetworkInterface->ReceiveMsg());

        while (mGameManager.HasEventToProcess()) {}
    }

    void RegisterAndGameStart(int roomId, 
        const std::vector<unsigned int> &userIds) 
    {
        EXPECT_CALL(*mMockStream, Write(_, _)).Times(AtLeast(1));
        mNetworkInterface->AsyncSendMsg(
            *MsgBuilder::CreateRegisterArgs(0, "uno", "UNO", 4, 2));
        ProcessMsgFromCore(*MsgBuilder::CreateRegisterRet(0, ErrorNumber::OK));

        /// TODO: check flippedCard and firstPlayerId with action
        EXPECT_CALL(*mMockStream, Write(
            AllOf(NotifyMsgArgsMatcher(ErrorNumber::OK, roomId, userIds[0]),
                GameStartMatcher()), _)).Times(1);

        EXPECT_CALL(*mMockStream, Write(
            AllOf(NotifyMsgArgsMatcher(ErrorNumber::OK, roomId, userIds[1]),
                GameStartMatcher()), _)).Times(1);

        EXPECT_CALL(*mMockStream, Write(
            AllOf(NotifyMsgArgsMatcher(ErrorNumber::OK, roomId, userIds[2]),
                GameStartMatcher()), _)).Times(1);
        
        ProcessMsgFromCore(*MsgBuilder::CreateStartGameArgs(0, roomId, userIds,
            MsgBuilder::CreateStartGameSettings(true, true, false, false, 15)));
    }

protected:
    MockClientReaderWriter<ProviderMsg, ProviderMsg> *mMockStream;
    std::shared_ptr<MockGameCoreStub> mMockStub;
    std::shared_ptr<StateMachine<GlobalState>> mStateMachine;
    std::shared_ptr<NetworkInterface<true>> mNetworkInterface;
    GameManager<GlobalState, true> mGameManager;
    std::unique_ptr<std::thread> mThread;
};

TEST_F(MockCoreFixture, RegisterAndGameStart) {
    auto roomId = 1002;
    std::vector<unsigned int> userIds = {11, 22, 33};
    RegisterAndGameStart(roomId, userIds);

    const auto &state = mStateMachine->GetState();
    EXPECT_EQ(state.mRoomIdToGameState.size(), 1);
    EXPECT_NE(state.mRoomIdToGameState.find(roomId), 
        state.mRoomIdToGameState.end());
    const auto &gameState = state.mRoomIdToGameState.at(roomId);
    EXPECT_EQ(gameState.mPlayerNum, userIds.size());
    EXPECT_EQ(gameState.mUserIdToPlayerState.size(), userIds.size());
    for (auto userId : userIds) {
        EXPECT_NE(gameState.mUserIdToPlayerState.find(userId), 
            gameState.mUserIdToPlayerState.end());
    }
    auto cardsInDeck = 108 - 7 * userIds.size() - 1;
    EXPECT_EQ(gameState.mDeck.Number(), cardsInDeck);
}

TEST_F(MockCoreFixture, Draw) {
    auto roomId = 1002;
    std::vector<unsigned int> userIds = {11, 22, 33};
    RegisterAndGameStart(roomId, userIds);

    auto drawNum = 1;
    EXPECT_CALL(*mMockStream, Write(
        AllOf(NotifyMsgArgsMatcher(ErrorNumber::OK, roomId, userIds[0]),
            DrawRspMatcher(drawNum)), _)).Times(1);
    EXPECT_CALL(*mMockStream, Write(
        AllOf(NotifyMsgArgsMatcher(ErrorNumber::OK, roomId, -userIds[0]),
            DrawMatcher(drawNum)), _)).Times(1);
    ProcessMsgFromCore(*MsgBuilder::CreateUserOperationArgs(0, roomId,
        userIds[0], MsgBuilder::CreateDraw(drawNum)));

    const auto &state = mStateMachine->GetState().mRoomIdToGameState.at(roomId);
    auto cardsInDeck = 108 - 7 * userIds.size() - 1;
    EXPECT_EQ(state.mDeck.Number(), cardsInDeck - 1);
    EXPECT_EQ(state.mDiscardPile.Number(), 0);
    EXPECT_EQ(state.mUserIdToPlayerState.at(userIds[0]).mHandcards.Number(), 8);
    EXPECT_EQ(state.mUserIdToPlayerState.at(userIds[1]).mHandcards.Number(), 7);
    EXPECT_EQ(state.mUserIdToPlayerState.at(userIds[2]).mHandcards.Number(), 7);
}

TEST_F(MockCoreFixture, Skip) {
    auto roomId = 1002;
    std::vector<unsigned int> userIds = {11, 22, 33};
    RegisterAndGameStart(roomId, userIds);

    EXPECT_CALL(*mMockStream, Write(
        AllOf(NotifyMsgArgsMatcher(ErrorNumber::OK, roomId, -userIds[1]),
            SkipMatcher()), _)).Times(1);
    ProcessMsgFromCore(*MsgBuilder::CreateUserOperationArgs(0, roomId,
        userIds[1], MsgBuilder::CreateSkip()));

    const auto &state = mStateMachine->GetState().mRoomIdToGameState.at(roomId);
    auto cardsInDeck = 108 - 7 * userIds.size() - 1;
    EXPECT_EQ(state.mDeck.Number(), cardsInDeck);
    EXPECT_EQ(state.mDiscardPile.Number(), 0);
    EXPECT_EQ(state.mUserIdToPlayerState.at(userIds[0]).mHandcards.Number(), 7);
    EXPECT_EQ(state.mUserIdToPlayerState.at(userIds[1]).mHandcards.Number(), 7);
    EXPECT_EQ(state.mUserIdToPlayerState.at(userIds[2]).mHandcards.Number(), 7);
}

TEST_F(MockCoreFixture, Play) {
    auto roomId = 1002;
    std::vector<unsigned int> userIds = {11, 22, 33};
    RegisterAndGameStart(roomId, userIds);

    const auto &state = mStateMachine->GetState().mRoomIdToGameState.at(roomId);
    const auto &playerState = state.mUserIdToPlayerState.at(userIds[2]);
    auto card = playerState.mHandcards.At(3);
    CardColor color = card.mColor;
    EXPECT_CALL(*mMockStream, Write(
        AllOf(NotifyMsgArgsMatcher(ErrorNumber::OK, roomId, -userIds[2]),
            PlayMatcher(card, color)), _)).Times(1);
    ProcessMsgFromCore(*MsgBuilder::CreateUserOperationArgs(0, roomId,
        userIds[2], MsgBuilder::CreatePlay(card, color)));
    
    auto cardsInDeck = 108 - 7 * userIds.size() - 1;
    EXPECT_EQ(state.mDeck.Number(), cardsInDeck);
    EXPECT_EQ(state.mDiscardPile.Number(), 1);
    EXPECT_EQ(state.mUserIdToPlayerState.at(userIds[0]).mHandcards.Number(), 7);
    EXPECT_EQ(state.mUserIdToPlayerState.at(userIds[1]).mHandcards.Number(), 7);
    EXPECT_EQ(state.mUserIdToPlayerState.at(userIds[2]).mHandcards.Number(), 6);
}

}}
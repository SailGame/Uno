#include <gtest/gtest.h>
#include <grpcpp/test/mock_stream.h>

#include <sailgame_pb/core/core_mock.grpc.pb.h>
#include <sailgame_pb/core/provider.pb.h>
#include <sailgame_pb/core/error.pb.h>
#include <sailgame_pb/uno/uno.pb.h>

#include <sailgame/uno/msg_builder.h>
#include <sailgame/common/game_manager.h>
#include <sailgame/common/provider_msg_builder.h>
#include <sailgame/common/util.h>

#include "../src/state.h"
#include "../src/state_machine.h"

namespace SailGame { namespace Test {

using namespace testing;
using Common::EventLoop;
using Common::ProviderGameManager;
using Common::ProviderStateMachine;
using Common::ProviderNetworkInterface;
using Common::ProviderMsgBuilder;
using Common::Util;
using ::Core::ErrorNumber;
using ::Core::MockGameCoreStub;
using ::Core::ProviderMsg;
using grpc::testing::MockClientReaderWriter;
using SailGame::Uno::Card;
using SailGame::Uno::GlobalState;
using SailGame::Uno::MsgBuilder;
using SailGame::Uno::StateMachine;
using SailGame::Uno::GlobalState;
using ::Uno::CardColor;
using ::Uno::CardText;
using ::Uno::GameStart;
using ::Uno::NotifyMsg;
using ::Uno::UserOperation;

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
        mStateMachine(std::make_shared<StateMachine>()),
        mNetworkInterface(ProviderNetworkInterface::Create(mMockStub)),
        mGameManager(EventLoop::Create(), mStateMachine, mNetworkInterface)
    {}

    void SetUp() {
        // spdlog::set_level(spdlog::level::err);
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
            ProviderMsgBuilder::CreateRegisterArgs(0, "uno", "UNO", 4, 2));
        ProcessMsgFromCore(ProviderMsgBuilder::CreateRegisterRet(0, ErrorNumber::OK));

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
        
        ProcessMsgFromCore(ProviderMsgBuilder::CreateStartGameArgs(0, roomId, userIds,
            MsgBuilder::CreateStartGameSettings(true, true, false, false, 15)));
    }

    GlobalState GetState() const {
        return dynamic_cast<const GlobalState &>(mStateMachine->GetState());
    }

protected:
    MockClientReaderWriter<ProviderMsg, ProviderMsg> *mMockStream;
    std::shared_ptr<MockGameCoreStub> mMockStub;
    std::shared_ptr<ProviderStateMachine> mStateMachine;
    std::shared_ptr<ProviderNetworkInterface> mNetworkInterface;
    ProviderGameManager mGameManager;
    std::unique_ptr<std::thread> mThread;
};

TEST_F(MockCoreFixture, RegisterAndGameStart) {
    auto roomId = 1002;
    std::vector<unsigned int> userIds = {11, 22, 33};
    RegisterAndGameStart(roomId, userIds);

    // const auto &state = mStateMachine->GetState();
    EXPECT_EQ(GetState().mRoomIdToGameState.size(), 1);
    EXPECT_NE(GetState().mRoomIdToGameState.find(roomId), 
        GetState().mRoomIdToGameState.end());
    const auto &gameState = GetState().mRoomIdToGameState.at(roomId);
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
        AllOf(NotifyMsgArgsMatcher(ErrorNumber::OK, roomId, 0),
            DrawMatcher(drawNum)), _)).Times(1);
    ProcessMsgFromCore(ProviderMsgBuilder::CreateUserOperationArgs(0, roomId,
        userIds[0], MsgBuilder::CreateDraw<UserOperation>(drawNum)));

    const auto &state = GetState().mRoomIdToGameState.at(roomId);
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
        AllOf(NotifyMsgArgsMatcher(ErrorNumber::OK, roomId, 0),
            SkipMatcher()), _)).Times(1);
    ProcessMsgFromCore(ProviderMsgBuilder::CreateUserOperationArgs(0, roomId,
        userIds[1], MsgBuilder::CreateSkip<UserOperation>()));

    const auto &state = GetState().mRoomIdToGameState.at(roomId);
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

    const auto &playerState = GetState().mRoomIdToGameState.at(roomId)
        .mUserIdToPlayerState.at(userIds[2]);
    auto card = playerState.mHandcards.At(3);
    CardColor color = card.mColor;
    EXPECT_CALL(*mMockStream, Write(
        AllOf(NotifyMsgArgsMatcher(ErrorNumber::OK, roomId, 0),
            PlayMatcher(card, color)), _)).Times(1);
    ProcessMsgFromCore(ProviderMsgBuilder::CreateUserOperationArgs(0, roomId,
        userIds[2], MsgBuilder::CreatePlay<UserOperation>(card, color)));
    
    auto cardsInDeck = 108 - 7 * userIds.size() - 1;
    const auto &state = GetState().mRoomIdToGameState.at(roomId);
    EXPECT_EQ(state.mDeck.Number(), cardsInDeck);
    EXPECT_EQ(state.mDiscardPile.Number(), 1);
    EXPECT_EQ(state.mUserIdToPlayerState.at(userIds[0]).mHandcards.Number(), 7);
    EXPECT_EQ(state.mUserIdToPlayerState.at(userIds[1]).mHandcards.Number(), 7);
    EXPECT_EQ(state.mUserIdToPlayerState.at(userIds[2]).mHandcards.Number(), 6);
}

}}
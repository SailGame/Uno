#include <gtest/gtest.h>
#include <grpcpp/test/mock_stream.h>

#include <core/core_mock.grpc.pb.h>
#include <core/provider.pb.h>
#include <core/error.pb.h>
#include <uno/uno.pb.h>

#include "../src/game/msg_builder.h"
#include "../src/common/game_manager.h"
#include "../src/game/state.h"
#include "../src/game/msg_builder.h"
#include "../src/common/util.h"

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
using grpc::testing::MockClientReaderWriter;
using ::Uno::GameStart;
using ::Uno::NotifyMsg;

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

class MockCoreFixture : public ::testing::Test {
public:
    MockCoreFixture()
        : mMockStream(new MockClientReaderWriter<ProviderMsg, ProviderMsg>()),
        mMockStub(std::make_shared<MockGameCoreStub>()),
        mStateMachine(std::make_shared<StateMachine<GlobalState>>()),
        mNetworkInterface(NetworkInterface::Create(mMockStub)),
        mGameManager(EventLoop::Create(), mStateMachine, mNetworkInterface)
    {}

    void SetUp() {
        EXPECT_CALL(*mMockStub, ProviderRaw(_)).Times(AtLeast(1)).WillOnce(Return(mMockStream));
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

    void RegisterAndGameStart(int roomId, const std::vector<unsigned int> &userIds) {
        EXPECT_CALL(*mMockStream, Write(_, _)).Times(AtLeast(1));
        mNetworkInterface->SendMsg(
            *MsgBuilder::CreateRegisterArgs(0, "uno", "UNO", 4, 2));
        ProcessMsgFromCore(*MsgBuilder::CreateRegisterRet(0, ErrorNumber::OK));

        /// TODO: check flippedCard and firstPlayerId with action
        EXPECT_CALL(*mMockStream, Write(
            AllOf(
                NotifyMsgArgsMatcher(ErrorNumber::OK, roomId, userIds[0]),
                GameStartMatcher()
            ), _)
        ).Times(1);

        EXPECT_CALL(*mMockStream, Write(
            AllOf(
                NotifyMsgArgsMatcher(ErrorNumber::OK, roomId, userIds[1]),
                GameStartMatcher()
            ), _)
        ).Times(1);

        EXPECT_CALL(*mMockStream, Write(
            AllOf(
                NotifyMsgArgsMatcher(ErrorNumber::OK, roomId, userIds[2]),
                GameStartMatcher()
            ), _)
        ).Times(1);
        
        ProcessMsgFromCore(*MsgBuilder::CreateStartGameArgs(0, roomId, userIds,
            MsgBuilder::CreateStartGameSettings(true, true, false, false, 15)));
    }

protected:
    MockClientReaderWriter<ProviderMsg, ProviderMsg> *mMockStream;
    std::shared_ptr<MockGameCoreStub> mMockStub;
    std::shared_ptr<StateMachine<GlobalState>> mStateMachine;
    std::shared_ptr<NetworkInterface> mNetworkInterface;
    GameManager<GlobalState> mGameManager;
    std::unique_ptr<std::thread> mThread;
};

TEST_F(MockCoreFixture, RegisterAndGameStart) {
    auto roomId = 1002;
    std::vector<unsigned int> userIds = {11, 22, 33};
    RegisterAndGameStart(roomId, userIds);

    const auto &state = mStateMachine->GetState();
    EXPECT_EQ(state.mRoomIdToGameState.size(), 1);
    EXPECT_NE(state.mRoomIdToGameState.find(roomId), state.mRoomIdToGameState.end());
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

}}
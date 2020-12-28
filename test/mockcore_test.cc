#include <gtest/gtest.h>
#include <grpcpp/test/mock_stream.h>

#include <core/core_mock.grpc.pb.h>
#include <core/provider.pb.h>
#include <core/error.pb.h>

#include "../src/game/msg_builder.h"
#include "../src/common/game_manager.h"
#include "../src/game/state.h"
#include "../src/game/msg_builder.h"

namespace SailGame { namespace Test {

using namespace testing;
using Core::ErrorNumber;
using Common::GameManager;
using Common::EventLoop;
using Common::NetworkInterface;
using Common::StateMachine;
using Core::MockGameCoreStub;
using Core::ProviderMsg;
using Game::GlobalState;
using Game::MsgBuilder;
using grpc::testing::MockClientReaderWriter;

class MockCoreFixture : public ::testing::Test {
public:
    MockCoreFixture()
        : mMockStream(new MockClientReaderWriter<ProviderMsg, ProviderMsg>()),
        mNetworkInterface(NetworkInterface::Create(mMockStub)),
        mGameManager(EventLoop::Create(), StateMachine<GlobalState>::Create(),
            mNetworkInterface)
    {}

    void SetUp() {
        EXPECT_CALL(mMockStub, ProviderRaw(_)).Times(AtLeast(1)).WillOnce(Return(mMockStream));
        mThread = std::make_unique<std::thread>([&] {
            mGameManager.Start();
        });

        while (!mNetworkInterface->IsConnected()) {}
    }

    void TearDown() {
        mGameManager.Stop();
        mThread->join();
    }

    void ProcessMsg(const ProviderMsg &msg) {
        EXPECT_CALL(*mMockStream, Read(_)).Times(1).WillOnce(
            DoAll(SetArgPointee<0>(msg), Return(true)));
        mNetworkInterface->OnEventHappens(mNetworkInterface->ReceiveMsg());

        while (mGameManager.HasEventToProcess()) {}
    }

protected:
    MockClientReaderWriter<ProviderMsg, ProviderMsg> *mMockStream;
    MockGameCoreStub mMockStub;
    std::shared_ptr<NetworkInterface> mNetworkInterface;
    GameManager<GlobalState> mGameManager;
    std::unique_ptr<std::thread> mThread;
};

TEST_F(MockCoreFixture, Register) {
    EXPECT_CALL(*mMockStream, Write(_, _)).Times(1);
    mNetworkInterface->SendMsg(
        *MsgBuilder::CreateRegisterArgs(0, "uno", "UNO", 4, 2));

    ProcessMsg(*MsgBuilder::CreateRegisterRet(0, ErrorNumber::OK));
}

TEST_F(MockCoreFixture, GameStart) {

}

}}
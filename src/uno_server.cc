#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <sailgame/common/game_manager.h>
#include <sailgame/common/provider_msg_builder.h>

#include "state.h"
#include "state_machine.h"

using Core::ProviderMsg;
using Core::RegisterArgs;
using SailGame::Uno::GlobalState;
using SailGame::Uno::StateMachine;
using SailGame::Common::GameManager;
using SailGame::Common::EventLoop;
using SailGame::Common::NetworkInterface;
using SailGame::Common::ProviderMsgBuilder;

int main(int argc, char** argv) {
    spdlog::info("Hello, I'm Uno Server!");

    std::string endpoint = "localhost:50051";
    auto stub = NetworkInterface<true>::CreateStub(endpoint);
    GameManager<true> gameManager(
        EventLoop::Create(),
        StateMachine::Create(),
        NetworkInterface<true>::Create(stub));
        
    gameManager.StartWithRegisterArgs(
        ProviderMsgBuilder::CreateRegisterArgs(0, "uno", "UNO", 4, 2));

    return 0;
}

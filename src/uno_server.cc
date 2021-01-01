#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <sailgame/common/game_manager.h>
#include <sailgame/uno/msg_builder.h>

#include "state.h"

using Core::ProviderMsg;
using Core::RegisterArgs;
using SailGame::Game::MsgBuilder;
using SailGame::Common::GameManager;
using SailGame::Common::EventLoop;
using SailGame::Common::NetworkInterface;
using SailGame::Common::StateMachine;
using SailGame::Game::GlobalState;

int main(int argc, char** argv) {
    spdlog::info("Hello, I'm Uno Server!");

    std::string endpoint = "localhost:50051";
    auto stub = NetworkInterface<true>::CreateStub(endpoint);
    GameManager<GlobalState, true> gameManager(
        EventLoop::Create(),
        StateMachine<GlobalState>::Create(),
        NetworkInterface<true>::Create(stub));
        
    gameManager.StartWithRegisterArgs(
        MsgBuilder::CreateRegisterArgs(0, "uno", "UNO", 4, 2));

    return 0;
}

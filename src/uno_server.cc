#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "common/game_manager.h"
#include "game/state.h"
#include "game/msg_builder.h"

using Core::ProviderMsg;
using Core::RegisterArgs;
using SailGame::Game::MsgBuilder;
using SailGame::Common::GameManager;
using SailGame::Game::GlobalState;

int main(int argc, char** argv) {
    spdlog::info("Hello, I'm Uno Server!");
    GameManager<GlobalState> gameManager("localhost:50051");
    gameManager.StartWithRegisterArgs(
        MsgBuilder::CreateRegisterArgs(0, "uno", "UNO", 4, 2));

    return 0;
}

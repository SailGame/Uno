#include <spdlog/spdlog.h>

#include "common/game_manager.h"
#include "game/state.h"

using Core::ProviderMsg;
using Core::RegisterArgs;
using SailGame::Common::GameManager;
using SailGame::Game::State;

int main(int argc, char** argv) {
    spdlog::info("Hello, I'm Uno Server!");
    GameManager<State> gameManager;
    gameManager.Start();

    ProviderMsg msg;
    auto registerArgs = msg.mutable_registerargs();
    registerArgs->set_id("uno");
    registerArgs->set_gamename("UNO");
    auto gameSettings = registerArgs->mutable_gamesetting();
    gameSettings->set_minusers(2);
    gameSettings->set_minusers(4);

    gameManager.ProcessEvent(std::make_shared<ProviderMsg>(msg));

    return 0;
}

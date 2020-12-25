#include <spdlog/spdlog.h>

#include "common/game_manager.h"
#include "game/state.h"

using SailGame::Common::GameManager;
using SailGame::Game::State;

int main(int argc, char** argv) {
    spdlog::info("Hello, I'm Uno Server!");
    GameManager<State> gameManager;
    gameManager.Start();

    return 0;
}

#include <spdlog/spdlog.h>

#include "game/server_side/game_manager_s.h"

using SailGame::Game::GameManager;

int main(int argc, char** argv) {
    spdlog::info("Hello, I'm Uno Server!");
    GameManager gameManager;
    gameManager.Start();

    return 0;
}

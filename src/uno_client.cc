#include "game/client_side/game_manager_c.h"

using SailGame::Game::GameManager;

int main(int argc, char** argv) {
    GameManager gameManager;
    gameManager.Start();

    return 0;
}

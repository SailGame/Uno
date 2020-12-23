#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "game/server_side/game_manager_s.h"

using SailGame::Game::GameManager;

int main(int argc, char** argv) {
    GameManager gameManager;
    gameManager.Start();

    return 0;
}

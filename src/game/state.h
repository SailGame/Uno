#pragma once

#include <map>
#include <vector>

#include <uno/uno.pb.h>

#include "card.h"

namespace SailGame { namespace Game {

using ::Uno::StartGameSettings;

class PlayerState {
public:
    PlayerState() : mHandcards(std::make_unique<Handcards>()) {}

public:
    std::unique_ptr<Handcards> mHandcards;
};

class GameState {
public:
    using GameStartInfo = std::tuple<
        std::map<unsigned int, InitHandcardsT>,
        Card,
        unsigned int
    >;

public: 
    GameState(const std::vector<unsigned int> &userIds,
        const StartGameSettings &gameSettings);

    GameStartInfo GameStart();

public:
    int mPlayerNum;
    std::unique_ptr<StartGameSettings> mGameSettings;
    std::map<int, PlayerState> mUserIdToPlayerState;
    std::unique_ptr<Deck> mDeck;
    std::unique_ptr<DiscardPile> mDiscardPile;
};

class GlobalState {
public:
    GameState::GameStartInfo NewGame(int roomId, 
        const std::vector<unsigned int> &userIds,
        const StartGameSettings &gameSettings);

public:
    std::map<int, GameState> mRoomIdToGameState;
    int mCurrentRoomId{-1};
    int mCurrentUserId{-1};
};

std::ostream &operator<<(std::ostream &os, const PlayerState &playerState);
std::ostream &operator<<(std::ostream &os, const GameState &gameState);
std::ostream &operator<<(std::ostream &os, const GlobalState &globalState);

/// TODO: mapping from usedId to playerIndex should be managed in client side
// std::map<int, int> mPlayerIndexToUserId;

}}
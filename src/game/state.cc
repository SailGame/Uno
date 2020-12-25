#include <ctime>

#include "state.h"

namespace SailGame { namespace Game {

GameState::GameState(const std::vector<unsigned int> &userIds, 
    const StartGameSettings &gameSettings)
    : mPlayerNum(userIds.size()),
    mGameSettings(std::make_unique<StartGameSettings>(gameSettings))
{
    for (auto userId : userIds) {
        mUserIdToPlayerState.emplace(userId, PlayerState{});
    }

    mDiscardPile = std::make_unique<DiscardPile>();
    mDeck = std::make_unique<Deck>(*mDiscardPile);
    
    mDeck->Init();
}

GameState::GameStartInfo GameState::GameStart()
{
    // step 1. deal init handcards
    auto initHandcards = mDeck->DealInitHandcards(mPlayerNum);
    std::map<unsigned int, InitHandcardsT> userIdToInitHandcards;
    auto cur = 0;
    for (auto &playerState : mUserIdToPlayerState) {
        auto userId = playerState.first;
        playerState.second.mHandcards->FillInitHandcards(initHandcards[cur]);
        userIdToInitHandcards.emplace(userId, initHandcards[cur]);
        cur++;
    }
    assert(cur == mPlayerNum);

    // step 2. flip a card
    Card flippedCard;
    while (true) {
        flippedCard = mDeck->Draw();
        if (flippedCard.mColor == CardColor::BLACK) {
            // if the flipped card is a wild card, put it to under the deck and flip a new one
            mDeck->PutToBottom(flippedCard);
        }
        else {
            if (CardSet::DrawTexts.count(flippedCard.mText)) {
                // last played card will become EMPTY if the flipped card is `Draw` card
                flippedCard.mText = CardText::EMPTY;
            }
            break;
        }
    }

    // step 3. choose the first player randomly
    std::srand(std::time(nullptr));
    unsigned int firstPlayer = std::rand() % mPlayerNum;

    return {userIdToInitHandcards, flippedCard, firstPlayer};
}

GameState::GameStartInfo GlobalState::NewGame(int roomId, 
    const std::vector<unsigned int> &userIds, const StartGameSettings &gameSettings)
{
    // assume roomId doesn't exist
    assert(mRoomIdToGameState.find(roomId) == mRoomIdToGameState.end());
    mRoomIdToGameState.emplace(roomId, GameState{userIds, gameSettings});
    return mRoomIdToGameState.at(roomId).GameStart();
}
}}
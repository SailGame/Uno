#pragma once

#include "card.h"
#include "config.h"
#include "../common/util.h"

namespace SailGame { namespace Game {

using Common::Util;

class GameState {
public:
    void UpdateAfterDraw();

    void UpdateAfterSkip();

    void UpdateAfterPlay(Card card);

    void NextPlayer();

    bool IsMyTurn() const { return mCurrentPlayer == Config::mMyUserId; }

private:
    int mCurrentPlayer{-1};
    bool mIsInClockwise{true};
    Card mLastPlayedCard{};
    int mCardsNumToDraw{1};  // +2 and +4 can accumulate
    int mTimeElapsed{0};
};

class PlayerState {
public:
    void UpdateAfterDraw(int number);

    void UpdateAfterDraw(int number, int indexOfNewlyDrawn);

    void UpdateAfterSkip();

    void UpdateAfterPlay(Card card);

private:
    std::string mUsername{""};
    int mRemainingHandCardsNum{-1};
    bool mDoPlayInLastRound{false};
    Card mLastPlayedCard{};
    bool mHasChanceToPlayAfterDraw{false};
    int mIndexOfNewlyDrawn{-1};
};

class Handcards {
public:
    void Draw(Card card);

    void Draw(const std::vector<Card> &cards);

    void Erase(int index);

    int GetIndex(Card card) const;

    int GetIndexOfNewlyDrawn(const Handcards &handcardsBeforeDraw) const;

    bool Empty() const { return mCards.empty(); }

    Card At(int index) const { return *std::next(mCards.begin(), index); }

    int Number() const { return mCards.size(); }

private:
    std::multiset<Card> mCards;
};

struct State {
    GameState mGameState;
    std::vector<PlayerState> mPlayerStates;
    Handcards mHandcards;
};
}}
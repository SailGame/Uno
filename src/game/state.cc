#include "state.h"

namespace SailGame { namespace Game {

void GameState::UpdateAfterDraw()
{
    if (CardSet::DrawTexts.count(mLastPlayedCard.mText)) {
        // last played card will become EMPTY after the draw penalty is consumed
        mLastPlayedCard.mText = CardText::EMPTY;
    }
    // the number of cards to draw falls back to 1
    mCardsNumToDraw = 1;

    // no need to invoke NextPlayer() here 
    // because a draw action is always followed by a skip or play action
}

void GameState::UpdateAfterSkip()
{
    if (mLastPlayedCard.mText == CardText::SKIP) {
        // last played card will become EMPTY after the skip penalty is consumed
        mLastPlayedCard.mText = CardText::EMPTY;
    }
    NextPlayer();
}

void GameState::NextPlayer()
{
    mCurrentPlayer = mIsInClockwise ? 
        Util::Wrap(mCurrentPlayer + 1, Config::mPlayerNum) :
        Util::Wrap(mCurrentPlayer - 1, Config::mPlayerNum);
    mTimeElapsed = 0;
}

void GameState::UpdateAfterPlay(Card card)
{
    if (card.mText == CardText::WILD) {
        // if just a common wild card (not +4), don't affect the number text
        mLastPlayedCard.mColor = card.mColor;
    }
    else {
        mLastPlayedCard = card;
    }
    
    if (card.mText == CardText::REVERSE) {
        mIsInClockwise = !mIsInClockwise;
    }
    if (card.mText == CardText::DRAW_TWO) {
        // in the normal state, mCardsNumToDraw is equal to 1
        // once a player plays a `Draw` card, the effect is gonna accumulate
        mCardsNumToDraw = (mCardsNumToDraw == 1) ? 2 : (mCardsNumToDraw + 2);
    }
    if (card.mText == CardText::DRAW_FOUR) {
        mCardsNumToDraw = (mCardsNumToDraw == 1) ? 4 : (mCardsNumToDraw + 4);
    }

    NextPlayer();
}

void PlayerState::UpdateAfterDraw(int number)
{
    mRemainingHandCardsNum += number;
    mDoPlayInLastRound = false;
    // only common draw (rather than draw penalty due to +2 / +4) 
    // has the chance to play the card just drawn immediately
    mHasChanceToPlayAfterDraw = (number == 1);
}

void PlayerState::UpdateAfterDraw(int number, int indexOfNewlyDrawn)
{
    UpdateAfterDraw(number);
    mIndexOfNewlyDrawn = indexOfNewlyDrawn;
}

void PlayerState::UpdateAfterSkip()
{
    mDoPlayInLastRound = false;
    mHasChanceToPlayAfterDraw = false;
}

void PlayerState::UpdateAfterPlay(Card card)
{
    mRemainingHandCardsNum--;
    mDoPlayInLastRound = true;
    mLastPlayedCard = card;
    mHasChanceToPlayAfterDraw = false;
}

void Handcards::Draw(Card card)
{
    mCards.emplace(card);
}

void Handcards::Draw(const std::vector<Card> &cards)
{
    std::for_each(cards.begin(), cards.end(), [this](Card card) {
        Draw(card);
    });
}

void Handcards::Erase(int index)
{
    mCards.erase(std::next(mCards.begin(), index));
}

int Handcards::GetIndex(Card card) const
{
    auto it = mCards.find(card);
    assert(it != mCards.end());
    return std::distance(mCards.begin(), it);
}

int Handcards::GetIndexOfNewlyDrawn(const Handcards &handcardsBeforeDraw) const
{
    assert(Number() == handcardsBeforeDraw.Number() + 1);
    for (int i = 0; i < handcardsBeforeDraw.Number(); i++) {
        if (At(i) != handcardsBeforeDraw.At(i)) {
            return i;
        }
    }
    return handcardsBeforeDraw.Number();
}

}}
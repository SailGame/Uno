#include "card.h"

namespace SailGame { namespace Game {

const std::set<CardColor> CardSet::NonWildColors = 
    { CardColor::RED, CardColor::YELLOW, CardColor::GREEN, CardColor::BLUE };

const std::set<CardText> CardSet::NonWildTexts = 
    { CardText::ZERO, CardText::ONE, CardText::TWO, CardText::THREE, CardText::FOUR, 
      CardText::FIVE, CardText::SIX, CardText::SEVEN, CardText::EIGHT, CardText::NINE, 
      CardText::SKIP, CardText::REVERSE, CardText::DRAW_TWO };

const std::set<CardText> CardSet::DrawTexts = { CardText::DRAW_TWO, CardText::DRAW_FOUR };

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
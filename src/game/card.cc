#include "card.h"

namespace SailGame { namespace Game {

const std::set<CardColor> CardSet::NonWildColors = 
    { CardColor::RED, CardColor::YELLOW, CardColor::GREEN, CardColor::BLUE };

const std::set<CardText> CardSet::NonWildTexts = 
    { CardText::ZERO, CardText::ONE, CardText::TWO, CardText::THREE, CardText::FOUR, 
      CardText::FIVE, CardText::SIX, CardText::SEVEN, CardText::EIGHT, CardText::NINE, 
      CardText::SKIP, CardText::REVERSE, CardText::DRAW_TWO };

const std::set<CardText> CardSet::DrawTexts = { CardText::DRAW_TWO, CardText::DRAW_FOUR };

}}
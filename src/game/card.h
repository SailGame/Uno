#pragma once

#include "hello.pb.h"

namespace SailGame { namespace Game {

using ::Uno::CardColor;
using ::Uno::CardText;

struct Card {
    CardColor mColor;
    CardText mText;

    Card() = default;
    Card(const ::Uno::Card &card) : mColor(card.color()), mText(card.text()) {}

    Card(const Card &) = default;

    bool operator<(const Card &rhs) const {
        return (mColor < rhs.mColor) || 
            (mColor == rhs.mColor && mText < rhs.mText);
    }
    
    bool operator==(const Card &rhs) const {
        return (mColor == rhs.mColor) && (mText == rhs.mText);
    }

    bool operator!=(const Card &card) const {
        return !(*this == card);
    }
};

struct CardSet {
    const static std::set<CardColor> NonWildColors;
    const static std::set<CardText> NonWildTexts;
    const static std::set<CardText> DrawTexts;
};

}}
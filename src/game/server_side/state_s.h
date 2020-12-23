#pragma once

#include "../card.h"
#include "../config.h"

namespace SailGame { namespace Game {

struct State {
    Deck mDeck;
    DiscardPile mDiscardPile;
    std::vector<Handcards> mHandcards;
    std::map<int, int> mPlayerIndexToUserId;
};
}}
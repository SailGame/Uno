#pragma once

#include <map>
#include <vector>

#include "card.h"
#include "config.h"

namespace SailGame { namespace Game {

struct State {
    DiscardPile mDiscardPile;
    Deck mDeck{mDiscardPile};
    std::vector<Handcards> mHandcards;
    std::map<int, int> mPlayerIndexToUserId;
};
}}
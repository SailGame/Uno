#pragma once

#include "card.h"
#include "config.h"

namespace SailGame { namespace Game {

class Util {
public:
    static int WrapWithPlayerNum(int numToWrap);

    static int Wrap(int numToWrap, int range);
};
}}
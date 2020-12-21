#include "util.h"

namespace SailGame { namespace Game {

int Util::WrapWithPlayerNum(int numToWrap)
{
    return Wrap(numToWrap, Config::mPlayerNum);
}

int Util::Wrap(int numToWrap, int range)
{
    int ret = numToWrap % range;
    if (ret < 0) {
        ret += range;
    }
    return ret;
}

}}
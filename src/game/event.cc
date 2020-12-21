#include "event.h"

namespace SailGame { namespace Game {

void UserInputEvent::Process(std::shared_ptr<State> &state)
{
    std::cout << mCharInputted << std::endl;
}

}}
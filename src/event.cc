#include "event.h"

namespace SailGame { namespace Common {

void UserInputEvent::Process(std::shared_ptr<Game::State> &state)
{
    std::cout << mCharInputted << std::endl;
}

}}
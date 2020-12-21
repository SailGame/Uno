#include "event.h"

namespace SailGame { namespace Game {

std::shared_ptr<NetworkEvent> NetworkEvent::Create(const NotifyMsg &msg)
{
    if (msg.has_draw()) {
        return std::make_shared<DrawNetworkEvent>(msg.draw());
    }
    return nullptr;
}

void UserInputEvent::Process(std::shared_ptr<State> &state)
{
    std::cout << mCharInputted << std::endl;
}

void TimerEvent::Process(std::shared_ptr<State> &state)
{
    std::cout << "time ticks" << std::endl;
}

void DrawNetworkEvent::Process(std::shared_ptr<State> &state)
{
    std::cout << mDraw.userid() << " " << mDraw.number() << std::endl;
}
}}
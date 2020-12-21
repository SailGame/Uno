#include "event_listener.h"

namespace SailGame { namespace Common {

using Game::NetworkEvent;
using Game::UserInputEvent;

void NetworkEventListener::operator()()
{
    while (true) {
        auto msg = mClient->Receive();
        OnEventHappens(NetworkEvent::Create(msg));
    }
}

void UserInputEventListener::operator()()
{
    auto id = ' ';
    while (true) {
        std::cin >> id;
        OnEventHappens(std::make_shared<UserInputEvent>(id));
    }
}

void TimerEventListener::operator()()
{
}

}}
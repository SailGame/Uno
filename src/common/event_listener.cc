#include "event_listener.h"

namespace SailGame { namespace Common {

using Game::NetworkEvent;
using Game::UserInputEvent;
using Game::TimerEvent;

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
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        OnEventHappens(std::make_shared<TimerEvent>());
    }
}

}}
#include "event_listener.h"

namespace SailGame { namespace Common {

using Game::NetworkEvent;
using Game::UserInputEvent;
using Game::TimerEvent;

void UserInputEventListener::operator()()
{
    while (true) {
        // auto ch = Util::GetCharImmediately();
        // OnEventHappens(UserInputEvent::Create(ch));
        // int a;
        // std::cin >> a;
        // OnEventHappens(std::make_shared<UserInputEvent>());
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
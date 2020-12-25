#pragma once

#include <functional>
#include <thread>

#include "../../common/event_listener.h"
#include "../../common/event_loop.h"
#include "../../common/state_machine.h"
#include "../../common/network_interface.h"
#include "../../ui/ui_manager.h"
#include "../../network/client.h"
#include "state_c.h"

namespace SailGame { namespace Game {

using Common::EventLoop;
using Common::NetworkInterface;
using Common::StateMachine;
using Network::Client;
using UI::UIManager;

class GameManager {
public:
    GameManager();

    void Start();

    void ProcessEvent(const std::shared_ptr<Event> &event);

private:
    std::unique_ptr<EventLoop> mEventLoop;
    std::unique_ptr<StateMachine<State>> mStateMachine;
    std::unique_ptr<NetworkInterface<Client>> mNetworkInterface;
    std::unique_ptr<UIManager> mUIManager;

    std::unique_ptr<std::thread> mUserInputThread;
    std::unique_ptr<std::thread> mTimerThread;
};
}}
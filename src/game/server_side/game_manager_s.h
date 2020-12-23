#pragma once

#include <functional>
#include <thread>

#include "../../common/event_listener.h"
#include "../../common/event_loop.h"
#include "../../common/state_machine.h"
#include "../../common/network_interface.h"
#include "../../network/server.h"
#include "state_s.h"

namespace SailGame { namespace Game {

using Common::EventLoop;
using Common::StateMachine;
using Common::NetworkInterface;
using Network::Server;

class GameManager {
public:
    GameManager();

    void Start();

    void ProcessEvent(const std::shared_ptr<Event> &event);

private:
    std::unique_ptr<EventLoop> mEventLoop;
    std::unique_ptr<StateMachine<State>> mStateMachine;
    std::unique_ptr<NetworkInterface<Server>> mNetworkInterface;
};
}}
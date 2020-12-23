#pragma once

#include <functional>
#include <thread>

#include "../../common/event_listener.h"
#include "../../common/event_loop.h"
#include "../../common/state_machine.h"
#include "../../ui/ui_manager.h"
#include "state_c.h"

namespace SailGame { namespace Game {

using Common::EventLoop;
using Common::StateMachine;
using UI::UIManager;

class GameManager {
public:
    GameManager();

    void Start();

private:
    std::unique_ptr<EventLoop> mEventLoop;
    std::unique_ptr<StateMachine<State>> mStateMachine;
    std::unique_ptr<UIManager> mUIManager;

    std::unique_ptr<std::thread> mNetworkThread;
    std::unique_ptr<std::thread> mUserInputThread;
    std::unique_ptr<std::thread> mTimerThread;
};
}}
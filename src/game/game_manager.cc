#include "game_manager.h"

namespace SailGame { namespace Game {

GameManager::GameManager()
    : mEventLoop(std::make_unique<EventLoop>()),
    mStateMachine(std::make_unique<StateMachine>()),
    mUIManager(std::make_unique<UIManager>())
{
    auto callback = [this](const std::shared_ptr<Event> &event) {
        std::cout << "event happens" << std::endl;
        mEventLoop->AppendEvent(event);
    };

    mNetworkThread = std::make_unique<std::thread>(NetworkEventListener{callback});
    mUserInputThread = std::make_unique<std::thread>(UserInputEventListener{callback});
    mTimerThread = std::make_unique<std::thread>(TimerEventListener{callback});
}

void GameManager::Start()
{
    mEventLoop->RegisterEventProcessedCallback(
        [this](const std::shared_ptr<Event> &event) {
            /// TODO: invoke state machine and ui manager
            mStateMachine->Transition(event);
        }
    );

    mEventLoop->StartLoop();
}
}}
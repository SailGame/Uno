#include "game_manager_c.h"

namespace SailGame { namespace Game {

using Common::NetworkEventListener;
using Common::UserInputEventListener;
using Common::TimerEventListener;

GameManager::GameManager()
    : mEventLoop(std::make_unique<EventLoop>()),
    mStateMachine(std::make_unique<StateMachine<State>>()),
    mUIManager(std::make_unique<UIManager>())
{
    auto callback = [this](const std::shared_ptr<Event> &event) {
        mEventLoop->AppendEvent(event);
    };

    mNetworkInterface = std::make_unique<NetworkInterface>(NetworkInterface{callback, "localhost:50051"});
    // mNetworkThread = std::make_unique<std::thread>(NetworkEventListener{callback, "localhost:50051"});
    mUserInputThread = std::make_unique<std::thread>(UserInputEventListener{callback});
    mTimerThread = std::make_unique<std::thread>(TimerEventListener{callback});
}

void GameManager::Start()
{
    mNetworkInterface->Listen();
    mEventLoop->RegisterEventProcessedCallback(
        [this](const std::shared_ptr<Event> &event) {
            ProcessEvent(event);
        }
    );

    mEventLoop->StartLoop();
}

void GameManager::ProcessEvent(const std::shared_ptr<Event> &event)
{
    auto userOperation = mStateMachine->Transition(event);
    /// TODO: invoke ui manager
    if (userOperation) {
        mNetworkInterface->Send(*userOperation);
    }
}

}}
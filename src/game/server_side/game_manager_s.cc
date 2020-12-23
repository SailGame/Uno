#include "game_manager_s.h"

namespace SailGame { namespace Game {

// using Common::NetworkEventListener;
using Common::NotifyMsgPtr;
using Common::TimerEventListener;
using Common::UserInputEventListener;

GameManager::GameManager()
    : mEventLoop(std::make_unique<EventLoop>()),
    mStateMachine(std::make_unique<StateMachine<State>>())
{
    auto callback = [this](const std::shared_ptr<Event> &event) {
        mEventLoop->AppendEvent(event);
    };

    mNetworkInterface = std::make_unique<NetworkInterface<Server>>(
        NetworkInterface<Server>{callback, 50051});
}

void GameManager::Start()
{
    mEventLoop->RegisterEventProcessedCallback(
        [this](const std::shared_ptr<Event> &event) {
            ProcessEvent(event);
        }
    );

    mEventLoop->StartLoop();
}

void GameManager::ProcessEvent(const std::shared_ptr<Event> &event)
{
    auto notifyMsg = mStateMachine->Transition(event);
    /// TODO: invoke ui manager
    if (notifyMsg) {
        /// XXX: use correct userId
        mNetworkInterface->SendMsg(0, std::get<NotifyMsg>(*notifyMsg));
    }
}

}}
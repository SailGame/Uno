#include "game_manager_s.h"

namespace SailGame { namespace Game {

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
    auto notifyMsgs = mStateMachine->Transition(event);
    assert(notifyMsgs.size() == Config::mPlayerNum);
    /// XXX: send them asynchronously at the same time (not sequentially) for fairness
    for (int i = 0; i < notifyMsgs.size(); i++) {
        const auto &msg = *std::get<NotifyMsgPtr>(notifyMsgs[i]);
        mNetworkInterface->SendMsg(i, msg);
    }
}

}}
#pragma once

#include <functional>
#include <thread>
#include <core/provider.pb.h>

#include "event_loop.h"
#include "network_interface.h"
#include "state_machine.h"

namespace SailGame { namespace Common {

using Core::ProviderMsg;

template<typename StateT>
class GameManager {
public:
    GameManager()
        : mStateMachine(std::make_unique<StateMachine<StateT>>())
    {
        auto eventHappensCallback = [this](const ProviderMsgPtr &event) {
            mEventLoop->AppendEvent(event);
        };
        mNetworkInterface = std::make_unique<NetworkInterface>(
            "localhost:8080", eventHappensCallback);

        auto eventProcessedCallback = [this](const ProviderMsgPtr &event) {
            ProcessEvent(event);
        };
        mEventLoop = std::make_unique<EventLoop>(eventProcessedCallback);
    }

    void Start() {
        mNetworkInterface->AsyncListen();
        mEventLoop->StartLoop();
    }

    void ProcessEvent(const ProviderMsgPtr &event) {
        auto notifyMsgs = mStateMachine->Transition(*event);
        for (const auto &msg : notifyMsgs) {
            mNetworkInterface->SendMsg(*msg);
        }
    }

private:
    std::unique_ptr<EventLoop> mEventLoop;
    std::unique_ptr<StateMachine<StateT>> mStateMachine;
    std::unique_ptr<NetworkInterface> mNetworkInterface;
};
}}
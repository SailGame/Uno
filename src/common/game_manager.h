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
class GameManager : 
    public EventLoopSubscriber, 
    public NetworkInterfaceSubscriber {
public:
    GameManager(const std::string &serverAddr)
        : mNetworkInterface(serverAddr)
    {
        mEventLoop.SetSubscriber(this);
        mNetworkInterface.SetSubscriber(this);
    }

    void StartWithRegisterArgs(const ProviderMsgPtr &msg) {
        mNetworkInterface.AsyncListen();
        mNetworkInterface.SendMsg(*msg);
        mEventLoop.StartLoop();
    }

    void OnEventHappens(const ProviderMsgPtr &event) override {
        mEventLoop.AppendEvent(event);
    }

    void OnEventProcessed(const ProviderMsgPtr &event) override {
        auto notifyMsgs = mStateMachine.Transition(*event);
        for (const auto &msg : notifyMsgs) {
            mNetworkInterface.SendMsg(*msg);
        }
    }

private:
    EventLoop mEventLoop;
    StateMachine<StateT> mStateMachine;
    NetworkInterface mNetworkInterface;
};
}}
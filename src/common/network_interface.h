#pragma once

#include <string>
#include <memory>
#include <functional>
#include <core/provider.pb.h>

#include "types.h"
#include "../network/client.h"

namespace SailGame { namespace Common {

using Network::Client;
using Core::ProviderMsg;

class NetworkInterfaceSubscriber {
public:
    virtual void OnEventHappens(const ProviderMsgPtr &) = 0;
};

class NetworkInterface {
public:
    NetworkInterface(const std::string &serverAddr) 
        : mClient(serverAddr, [this](const ProviderMsg &msg) { ProcessMsg(msg); })
    {}

    void AsyncListen() {
        mClient.Connect();
        /// XXX: remove this
        // mStream->WritesDone();

        auto listenFunc = [this] {
            while (true) {
                auto msg = mClient.Receive();
                ProcessMsg(msg);
            }
        };

        /// TODO: where to join the thread
        mListenThread = std::make_unique<std::thread>(listenFunc);
        spdlog::info("listen thread created");
    }

    void SendMsg(const ProviderMsg &msg) {
        mClient.Send(msg);
        spdlog::info("msg sent, type = {}", msg.Msg_case());
    }

    void ProcessMsg(const ProviderMsg &msg) {
        mSubscriber->OnEventHappens(std::make_shared<ProviderMsg>(msg));
        spdlog::info("msg received, type = {}", msg.Msg_case());
    }

    void SetSubscriber(NetworkInterfaceSubscriber *subscriber) {
        mSubscriber = subscriber;
    }

private:
    Client mClient;
    std::unique_ptr<std::thread> mListenThread;
    NetworkInterfaceSubscriber *mSubscriber{nullptr};
};
}}
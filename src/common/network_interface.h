#pragma once

#include <thread>
#include "../game/event.h"
#include "../network/client.h"

namespace SailGame { namespace Common {

using Game::Event;
using Game::NetworkEvent;
using Network::Client;
using Uno::UserOperation;

class NetworkInterface {
public:
    NetworkInterface(const std::function<void(const std::shared_ptr<NetworkEvent> &)> &callback,
        const std::string &serverAddr);

    void Listen();

    void Send(const UserOperation &msg);

private:
    std::function<void(const std::shared_ptr<NetworkEvent> &)> OnEventHappens;

private:
    std::unique_ptr<Client> mClient;
    std::unique_ptr<std::thread> mListenThread;
};
}}
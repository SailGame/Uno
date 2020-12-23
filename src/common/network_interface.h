#pragma once

#include <thread>
#include <type_traits>
#include "../game/event.h"
#include "../network/client.h"
#include "../network/server.h"

namespace SailGame { namespace Common {

using Game::Event;
using Game::NetworkEvent;
using Network::Client;
using Network::Server;
using Uno::UserOperation;
using Uno::NotifyMsg;

template<typename PeerT>
class NetworkInterface {
public:
    NetworkInterface(const std::function<void(const std::shared_ptr<NetworkEvent> &)> &callback,
        const std::string &serverAddr) 
        : OnEventHappens(callback),
        mPeer(std::make_unique<Client>(serverAddr, 
            [this](const NotifyMsg &msg) { ProcessMsg(msg); }))
    {
        static_assert(std::is_same_v<PeerT, Client>);
    }

    NetworkInterface(const std::function<void(const std::shared_ptr<NetworkEvent> &)> &callback,
        int port) 
        : OnEventHappens(callback),
        mPeer(std::make_unique<Server>(port, 
            [this](const UserOperation &msg) { ProcessMsg(msg); }))
    {
        static_assert(std::is_same_v<PeerT, Server>);
    }

    template<typename MsgT>
    void SendMsg(const MsgT &msg) {
        static_assert(std::is_same_v<PeerT, Client>);
        mPeer->Send(msg);
    }

    template<typename MsgT>
    void SendMsg(int userId, const MsgT &msg) {
        static_assert(std::is_same_v<PeerT, Server>);
        mPeer->Send(userId, msg);
    }

    template<typename MsgT>
    void ProcessMsg(const MsgT &msg) {
        OnEventHappens(NetworkEvent::Create(msg));
    }

private:
    std::function<void(const std::shared_ptr<NetworkEvent> &)> OnEventHappens;

private:
    std::unique_ptr<PeerT> mPeer;
};
}}
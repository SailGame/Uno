#include "network_interface.h"

namespace SailGame { namespace Common {

NetworkInterface::NetworkInterface(
    const std::function<void(const std::shared_ptr<NetworkEvent> &)> &callback, 
    const std::string &serverAddr
)
    : OnEventHappens(callback), mClient(std::make_unique<Client>(serverAddr))
{}

void NetworkInterface::Listen()
{
    auto listenFunc = [this] {
        while (true) {
            auto msg = mClient->Receive();
            OnEventHappens(NetworkEvent::Create(msg));
        }
    };

    /// TODO: where to join the thread
    mListenThread = std::make_unique<std::thread>(listenFunc);
}

void NetworkInterface::Send(const UserOperation &msg)
{
    mClient->Send(msg);
}

}}
#include "client.h"

namespace SailGame { namespace Network {

Client::Client(const std::string &endpoint,
    const std::function<void(const NotifyMsg &)> &newMsgCallback)
    : OnNewMsg(newMsgCallback), 
    mChannel(grpc::CreateChannel(endpoint, grpc::InsecureChannelCredentials())),
    mContext(std::make_shared<ClientContext>())
{}

void Client::Connect()
{
    std::unique_ptr<UnoService::Stub> stub(UnoService::NewStub(mChannel));
    mStream = stub->BiStream(mContext.get());

    /// XXX: remove this
    // mStream->WritesDone();

    auto listenFunc = [this] {
        NotifyMsg msg;
        while (mStream->Read(&msg)) {
            OnNewMsg(msg);
        }
        if (!mStream->Finish().ok()) {
            std::cout << "RouteChat rpc failed." << std::endl;
            std::exit(-1);
        }
    };

    /// TODO: where to join the thread
    mListenThread = std::make_unique<std::thread>(listenFunc);
}

void Client::Send(const UserOperation &msg)
{
    std::cout << "send: " << msg.Operation_case() << std::endl;
    mStream->Write(msg);
}
}}
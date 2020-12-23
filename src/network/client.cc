#include "client.h"

namespace SailGame { namespace Network {

Client::Client(const std::string &endpoint)
{
    std::shared_ptr<Channel> channel(grpc::CreateChannel(
        endpoint, grpc::InsecureChannelCredentials()));
    std::unique_ptr<UnoService::Stub> stub(UnoService::NewStub(channel));

    mContext = std::make_shared<ClientContext>();
    mStream = stub->BiStream(mContext.get());

    mStream->WritesDone();
}

NotifyMsg Client::Receive()
{
    NotifyMsg msg;
    if (mStream->Read(&msg)) {
        return msg;
    }

    if (!mStream->Finish().ok()) {
        std::cout << "RouteChat rpc failed." << std::endl;
    }
    std::exit(-1);
}

void Client::Send(const UserOperation &msg)
{
    mStream->Write(msg);
}
}}
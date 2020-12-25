#include "client.h"

namespace SailGame { namespace Network {

Client::Client(const std::string &endpoint, const OnNewMsgT &newMsgCallback)
    : OnNewMsg(newMsgCallback), 
    mChannel(grpc::CreateChannel(endpoint, grpc::InsecureChannelCredentials())),
    mStub(GameCore::NewStub(mChannel)),
    mContext(std::make_shared<ClientContext>())
{}

void Client::Connect()
{
    mStream = mStub->Provider(mContext.get());
}

ProviderMsg Client::Receive()
{
    ProviderMsg msg;
    if (mStream->Read(&msg)) {
        return msg;
    }
    if (!mStream->Finish().ok()) {
        std::cout << "RouteChat rpc failed." << std::endl;
        std::exit(-1);
    }
    assert(false);
    return msg;
}

void Client::Send(const ProviderMsg &msg)
{
    // std::cout << "send: " << msg.Operation_case() << std::endl;
    mStream->Write(msg);
}
}}
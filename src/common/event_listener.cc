#include "event_listener.h"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "hello.grpc.pb.h"

namespace SailGame { namespace Common {

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using Uno::NotifyMsg;
using Uno::UnoService;
using Uno::UserOperation;
using Game::NetworkEvent;
using Game::UserInputEvent;

void NetworkEventListener::operator()()
{
    std::shared_ptr<Channel> channel(grpc::CreateChannel("localhost:50051", 
        grpc::InsecureChannelCredentials()));
    std::unique_ptr<UnoService::Stub> stub_(UnoService::NewStub(channel));

    ClientContext context;

    std::shared_ptr<ClientReaderWriter<UserOperation, NotifyMsg> > stream(
        stub_->BiStream(&context));

    stream->WritesDone();

    NotifyMsg msg;
    while (stream->Read(&msg))
    {
        // std::cout << "Got message " << msg.payload() << std::endl;
        OnEventHappens(NetworkEvent::Create(msg));
    }

    Status status = stream->Finish();
    if (!status.ok()) {
        std::cout << "RouteChat rpc failed." << std::endl;
    }
}

void UserInputEventListener::operator()()
{
    auto id = ' ';
    while (true) {
        std::cin >> id;
        OnEventHappens(std::make_shared<UserInputEvent>(id));
    }
}

void TimerEventListener::operator()()
{
}

}}
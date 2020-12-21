#include "event_listener.h"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "hello.grpc.pb.h"

namespace SailGame { namespace Game {

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;

void NetworkEventListener::operator()()
{
    std::shared_ptr<Channel> channel(grpc::CreateChannel("localhost:50051", 
        grpc::InsecureChannelCredentials()));
    std::unique_ptr<Hello::Stub> stub_(Hello::NewStub(channel));

    ClientContext context;

    std::shared_ptr<ClientReaderWriter<IntWrapper, IntWrapper> > stream(
        stub_->BiStream(&context));

    stream->WritesDone();

    IntWrapper intWrapper;
    while (stream->Read(&intWrapper))
    {
        std::cout << "Got message " << intWrapper.payload() << std::endl;
        // OnEventHappens(std::make_shared<UserInputEvent>(id));
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
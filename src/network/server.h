#pragma once

#include <memory>
#include <thread>
#include <string>
#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#include "hello.grpc.pb.h"

namespace SailGame { namespace Network {

using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;
using std::chrono::system_clock;
using Uno::UserOperation;
using Uno::NotifyMsg;
using Uno::UnoService;

class HelloImpl;

class Server {
public:
    Server(int port, const std::function<void(const UserOperation &)> &newMsgCallback);

    void Start();

    void HandleNewConnection(ServerReaderWriter<NotifyMsg, UserOperation> *stream);

    void Send(int playerIndex, const NotifyMsg &msg);

private:
    std::function<void(const UserOperation &)> OnNewMsg;

private:
    std::unique_ptr<grpc::Server> mGrpcServer;
    std::unique_ptr<HelloImpl> mUnoService;
    std::vector<std::shared_ptr<ServerReaderWriter<NotifyMsg, UserOperation>>> mStreams;
    std::unique_ptr<std::thread> mListenThread;
};

class HelloImpl final : public UnoService::Service
{
public:
    explicit HelloImpl(
        const std::function<void(ServerReaderWriter<NotifyMsg, UserOperation> *)> &callback)
        : OnNewConnection(callback)
    {}

    Status BiStream(ServerContext *context,
                    ServerReaderWriter<NotifyMsg, UserOperation> *stream) override
    {
        std::cout << "new connection" << std::endl;
        OnNewConnection(stream);
        return Status::OK;
    }

private:
    std::function<void(ServerReaderWriter<NotifyMsg, UserOperation> *)> OnNewConnection;
};

}}
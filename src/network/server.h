#pragma once

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#include "hello.grpc.pb.h"

namespace SailGame { namespace Network {

// using grpc::Server;
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

class Server {
public:
    Server(int port, const std::function<void(const UserOperation &)> &newMsgCallback);

    void Start();

    void HandleNewConnection(int userId, 
        ServerReaderWriter<NotifyMsg, UserOperation> *stream);

    // UserOperation Receive(int userId);

    void Send(int userId, const NotifyMsg &msg);

private:
    std::function<void(const UserOperation &)> OnNewMsg;

private:
    std::unique_ptr<grpc::Server> mGrpcServer;

    std::map<int, std::shared_ptr<ServerReaderWriter<NotifyMsg, UserOperation>>> mStreams;
};

class HelloImpl final : public UnoService::Service
{
public:
    explicit HelloImpl(
        const std::function<void(int, ServerReaderWriter<NotifyMsg, UserOperation> *)> &callback)
        : OnNewConnection(callback)
    {}

    Status BiStream(ServerContext *context,
                    ServerReaderWriter<NotifyMsg, UserOperation> *stream) override
    {
        /// TODO: get the correct userId
        OnNewConnection(0, stream);
        // listenFunc();
        // while (true)
        // {
        //     UserOperation msg;
        //     stream->Read(&msg);
        //     // cout msg

        //     int userid = 0;
        //     int number = 0;
        //     std::cout << context << std::endl;
        //     std::cin >> userid >> number;
        //     Uno::NotifyMsg msg;
        //     Uno::Draw *draw = msg.mutable_draw();
        //     draw->set_userid(userid);
        //     draw->set_number(number);
        //     stream->Write(msg);
        // }

        return Status::OK;
    }

private:
    std::function<void(int, ServerReaderWriter<NotifyMsg, UserOperation> *)> OnNewConnection;
};

}}
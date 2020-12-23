#pragma once

#include <memory>
#include <thread>
#include <string>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "hello.grpc.pb.h"

namespace SailGame { namespace Network {

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using Uno::NotifyMsg;
using Uno::UnoService;
using Uno::UserOperation;

class Client {
public:
    Client(const std::string &endpoint, 
        const std::function<void(const NotifyMsg &)> &newMsgCallback);

    NotifyMsg Receive();

    void Send(const UserOperation &msg);

private:
    std::function<void(const NotifyMsg &)> OnNewMsg;

private:
    std::shared_ptr<ClientContext> mContext;
    std::shared_ptr<ClientReaderWriter<UserOperation, NotifyMsg>> mStream;
    std::unique_ptr<std::thread> mListenThread;
};
}}
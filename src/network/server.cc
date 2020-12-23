#include "server.h"

namespace SailGame { namespace Network {

Server::Server(int port,
    const std::function<void(const UserOperation &)> &newMsgCallback)
    : OnNewMsg(newMsgCallback)
{
    // port = 50051
    std::string server_address("0.0.0.0:" + std::to_string(port));

    HelloImpl service(
        [this](int userId, ServerReaderWriter<NotifyMsg, UserOperation> *stream) {
            HandleNewConnection(userId, stream);
        }
    );

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    mGrpcServer = builder.BuildAndStart();
    std::cout << "Server listening on " << server_address << std::endl;
}

void Server::Start()
{
    mGrpcServer->Wait();
}

void Server::HandleNewConnection(int userId, 
    ServerReaderWriter<NotifyMsg, UserOperation> *stream)
{
    mStreams[userId] = std::shared_ptr<ServerReaderWriter<NotifyMsg, UserOperation>>(stream);
    UserOperation msg;
    while (stream->Read(&msg)) {
        OnNewMsg(msg);
    }
}

// UserOperation Server::Receive(int userId)
// {
//     UserOperation msg;
//     if (mStreams[userId]->Read(&msg)) {
//         return msg;
//     }
//     std::cout << "server receive failure" << std::endl;
//     std::exit(-1);
// }

void Server::Send(int userId, const NotifyMsg &msg)
{
    mStreams[userId]->Write(msg);
}
}}
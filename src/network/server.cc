#include "server.h"

namespace SailGame { namespace Network {

Server::Server(int port,
    const std::function<void(const UserOperation &)> &newMsgCallback)
    : OnNewMsg(newMsgCallback)
{
    mUnoService = std::make_unique<HelloImpl>(
        [this](ServerReaderWriter<NotifyMsg, UserOperation> *stream) {
            HandleNewConnection(stream);
        }
    );

    std::string serverAddr("0.0.0.0:" + std::to_string(port));
    ServerBuilder builder;
    builder.AddListeningPort(serverAddr, grpc::InsecureServerCredentials());
    builder.RegisterService(mUnoService.get());
    mGrpcServer = builder.BuildAndStart();
    std::cout << "Server listening on " << serverAddr << std::endl;
}

void Server::Start()
{
    auto listenFunc = [this] {
        mGrpcServer->Wait();
    };

    /// TODO: where to join the thread
    mListenThread = std::make_unique<std::thread>(listenFunc);
}

void Server::HandleNewConnection(ServerReaderWriter<NotifyMsg, UserOperation> *stream)
{
    std::cout << "[in] HandleNewConnection" << std::endl;
    // static int curPlayerIndex = 0;
    mStreams.push_back(std::shared_ptr<ServerReaderWriter<NotifyMsg, UserOperation>>(stream));

    UserOperation msg;
    stream->Read(&msg);
    // the first msg must be JoinGame
    assert(msg.has_joingame());
    /// TODO: handle mapping from playerIndex to userId
    auto userId = msg.joingame().userid();
    OnNewMsg(msg);

    std::cout << "new player, userId: " << userId << std::endl;

    while (stream->Read(&msg)) {
        OnNewMsg(msg);
    }
}

void Server::Send(int playerIndex, const NotifyMsg &msg)
{
    mStreams[playerIndex]->Write(msg);
}
}}
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "game/client_side/game_manager_c.h"

using SailGame::Game::GameManager;

int main(int argc, char** argv) {

    // std::thread t([&eventLoop] {
    //     auto id = ' ';
    //     while (true) {
    //         std::cin >> id;
    //         eventLoop->AppendEvent(std::make_shared<Common::UserInputEvent>(id));
    //     }
    // });

    // std::thread netThread([&eventLoop] {
    //     std::shared_ptr<Channel> channel(grpc::CreateChannel("localhost:50051", 
    //         grpc::InsecureChannelCredentials()));
    //     std::unique_ptr<Hello::Stub> stub_(Hello::NewStub(channel));

    //     ClientContext context;

    //     std::shared_ptr<ClientReaderWriter<IntWrapper, IntWrapper> > stream(
    //         stub_->BiStream(&context));

    //     stream->WritesDone();

    //     IntWrapper intWrapper;
    //     while (stream->Read(&intWrapper))
    //     {
    //         std::cout << "Got message " << intWrapper.payload() << std::endl;
    //     }

    //     Status status = stream->Finish();
    //     if (!status.ok()) {
    //         std::cout << "RouteChat rpc failed." << std::endl;
    //     }
    // });

    GameManager gameManager;
    gameManager.Start();

    // t.join();

    return 0;
}

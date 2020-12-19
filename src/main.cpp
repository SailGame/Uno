#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "event_loop.h"

using namespace SailGame;

int main(int argc, char** argv) {
    auto eventLoop = std::make_shared<Common::EventLoop>();

    std::thread t([&eventLoop] {
        auto id = 0;
        while (true) {
            std::cin >> id;
            eventLoop->AppendEvent(std::make_shared<Common::Event>(id));
        }
    });

    eventLoop->StartLoop();
    t.join();

    return 0;
}

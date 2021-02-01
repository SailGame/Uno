#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/fmt/ostr.h>

#include <sailgame/common/game_manager.h>
#include <sailgame/common/provider_msg_builder.h>

#include "state_machine.h"

using Core::ProviderMsg;
using Core::RegisterArgs;
using SailGame::Uno::GlobalState;
using SailGame::Uno::StateMachine;
using SailGame::Common::ProviderGameManager;
using SailGame::Common::EventLoop;
using SailGame::Common::ProviderNetworkInterface;
using SailGame::Common::ProviderMsgBuilder;

int main(int argc, char** argv) {
    spdlog::set_default_logger(spdlog::basic_logger_mt("Uno Provider", "uno.log"));
    spdlog::set_level(spdlog::level::info);
    spdlog::default_logger()->flush_on(spdlog::level::info);
    spdlog::info("Hello, I'm Uno Server!");

    std::string endpoint = argv[1];

    // std::string endpoint = "localhost:50051";
    auto stub = ProviderNetworkInterface::CreateStub(endpoint);
    ProviderGameManager gameManager(
        EventLoop::Create(),
        StateMachine::Create(),
        ProviderNetworkInterface::Create(stub));
        
    gameManager.StartWithRegisterArgs(
        ProviderMsgBuilder::CreateRegisterArgs(0, "uno", "UNO", 4, 2));

    return 0;
}

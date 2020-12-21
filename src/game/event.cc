#include "event.h"

namespace SailGame { namespace Game {

std::shared_ptr<NetworkEvent> NetworkEvent::Create(const NotifyMsg &msg)
{
    if (msg.has_draw()) {
        return std::make_shared<DrawNetworkEvent>(msg.draw());
    }
    if (msg.has_skip()) {
        return std::make_shared<SkipNetworkEvent>(msg.skip());
    }
    if (msg.has_play()) {
        return std::make_shared<PlayNetworkEvent>(msg.play());
    }
    if (msg.has_drawrsp()) {
        return std::make_shared<DrawRspNetworkEvent>(msg.drawrsp());
    }
    if (msg.has_uno()) {
        return std::make_shared<UnoNetworkEvent>(msg.uno());
    }
    if (msg.has_catch_()) {
        return std::make_shared<CatchNetworkEvent>(msg.catch_());
    }
    if (msg.has_doubt()) {
        return std::make_shared<DoubtNetworkEvent>(msg.doubt());
    }
    if (msg.has_doubtrsp()) {
        return std::make_shared<DoubtRspNetworkEvent>(msg.doubtrsp());
    }
    assert(false);
    return nullptr;
}

void UserInputEvent::Process(std::shared_ptr<State> &state)
{
    std::cout << mCharInputted << std::endl;
}

void TimerEvent::Process(std::shared_ptr<State> &state)
{
    std::cout << "time ticks" << std::endl;
}

void DrawNetworkEvent::Process(std::shared_ptr<State> &state)
{
    std::cout << mDraw.userid() << " " << mDraw.number() << std::endl;
}

void SkipNetworkEvent::Process(std::shared_ptr<State> &state)
{
}

void PlayNetworkEvent::Process(std::shared_ptr<State> &state)
{
}

void DrawRspNetworkEvent::Process(std::shared_ptr<State> &state)
{
}

void UnoNetworkEvent::Process(std::shared_ptr<State> &state)
{
}

void CatchNetworkEvent::Process(std::shared_ptr<State> &state)
{
}

void DoubtNetworkEvent::Process(std::shared_ptr<State> &state)
{
}

void DoubtRspNetworkEvent::Process(std::shared_ptr<State> &state)
{
}

}}
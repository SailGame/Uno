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

// -------------------- NetworkEvent ---------------------
void DrawNetworkEvent::Process(std::shared_ptr<State> &state)
{
    state->mGameState.UpdateAfterDraw();
    state->mPlayerStates[mDraw.userid()].UpdateAfterDraw(mDraw.number());
}

void SkipNetworkEvent::Process(std::shared_ptr<State> &state)
{
    state->mGameState.UpdateAfterSkip();
    state->mPlayerStates[mSkip.userid()].UpdateAfterSkip();
}

void PlayNetworkEvent::Process(std::shared_ptr<State> &state)
{
    Card card{mPlay.card()};
    state->mGameState.UpdateAfterPlay(card);
    state->mPlayerStates[mPlay.userid()].UpdateAfterPlay(card);
}

void DrawRspNetworkEvent::Process(std::shared_ptr<State> &state)
{
    auto &gameState = state->mGameState;
    assert(gameState.IsMyTurn());
    gameState.UpdateAfterDraw();

    auto &playerState = state->mPlayerStates[Config::mMyUserId];
    auto number = mDrawRsp.cards().size();

    auto handcardsBeforeDraw = state->mHandcards;
    for (auto card : mDrawRsp.cards()) {
        state->mHandcards.Draw(card);
    }
    if (number == 1) {
        playerState.UpdateAfterDraw(number, 
            state->mHandcards.GetIndexOfNewlyDrawn(handcardsBeforeDraw));
    }
    else {
        playerState.UpdateAfterDraw(number);
    }
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
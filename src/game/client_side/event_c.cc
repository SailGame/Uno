#include "../event.h"
#include "state_c.h"

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

std::shared_ptr<UserInputEvent> UserInputEvent::Create(char ch)
{
    switch (ch) {
        case ',':  return std::make_shared<CursorMoveLeftUserInputEvent>();
        case '.':  return std::make_shared<CursorMoveRightUserInputEvent>();
#if defined(__unix__) || defined(__APPLE__)
        case '\n': return std::make_shared<PlayUserInputEvent>();
#elif defined(_WIN32)
        case '\r': return std::make_shared<PlayUserInputEvent>();
#endif
        case ' ':  return std::make_shared<PassUserInputEvent>();
        case 'q': case 'Q': return std::make_shared<QuitUserInputEvent>();
        default: return std::make_shared<UserInputEvent>();
    }
    assert(false);
    return nullptr;
}

std::shared_ptr<UserOperation> UserInputEvent::Process(std::shared_ptr<State> &state)
{
    // default handler for UserInputEvent, do nothing
    return nullptr;
}

std::shared_ptr<UserOperation> TimerEvent::Process(std::shared_ptr<State> &state)
{
    std::cout << "time ticks" << std::endl;
    return nullptr;
}

// -------------------- NetworkEvent ---------------------
std::shared_ptr<UserOperation> DrawNetworkEvent::Process(std::shared_ptr<State> &state)
{
    // state->mGameState.UpdateAfterDraw();
    // state->mPlayerStates[mDraw.userid()].UpdateAfterDraw(mDraw.number());
    std::cout << mDraw.userid() << " " << mDraw.number() << std::endl;
    return nullptr;
}

std::shared_ptr<UserOperation> SkipNetworkEvent::Process(std::shared_ptr<State> &state)
{
    state->mGameState.UpdateAfterSkip();
    state->mPlayerStates[mSkip.userid()].UpdateAfterSkip();
    return nullptr;
}

std::shared_ptr<UserOperation> PlayNetworkEvent::Process(std::shared_ptr<State> &state)
{
    Card card{mPlay.card()};
    state->mGameState.UpdateAfterPlay(card);
    state->mPlayerStates[mPlay.userid()].UpdateAfterPlay(card);
    return nullptr;
}

std::shared_ptr<UserOperation> DrawRspNetworkEvent::Process(std::shared_ptr<State> &state)
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
    return nullptr;
}

std::shared_ptr<UserOperation> UnoNetworkEvent::Process(std::shared_ptr<State> &state)
{
    return nullptr;
}

std::shared_ptr<UserOperation> CatchNetworkEvent::Process(std::shared_ptr<State> &state)
{
    return nullptr;
}

std::shared_ptr<UserOperation> DoubtNetworkEvent::Process(std::shared_ptr<State> &state)
{
    return nullptr;
}

std::shared_ptr<UserOperation> DoubtRspNetworkEvent::Process(std::shared_ptr<State> &state)
{
    return nullptr;
}

// -------------------- UserInputEvent ---------------------
std::shared_ptr<UserOperation> CursorMoveLeftUserInputEvent::Process(std::shared_ptr<State> &state)
{
    return nullptr;
}

std::shared_ptr<UserOperation> CursorMoveRightUserInputEvent::Process(std::shared_ptr<State> &state)
{    
    return nullptr;
}

std::shared_ptr<UserOperation> PlayUserInputEvent::Process(std::shared_ptr<State> &state)
{    
    return nullptr;
}

std::shared_ptr<UserOperation> PassUserInputEvent::Process(std::shared_ptr<State> &state)
{    
    return nullptr;
}

std::shared_ptr<UserOperation> QuitUserInputEvent::Process(std::shared_ptr<State> &state)
{    
    return nullptr;
}

}}
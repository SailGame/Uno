#include "../event.h"
#include "state_c.h"

namespace SailGame { namespace Game {

std::shared_ptr<NetworkEvent> NetworkEvent::Create(const NotifyMsg &msg)
{
    switch (msg.Msg_case()) {
        case NotifyMsg::MsgCase::kDraw: 
            return std::make_shared<DrawNetworkEvent>(msg.draw());
        case NotifyMsg::MsgCase::kSkip: 
            return std::make_shared<SkipNetworkEvent>(msg.skip());
        case NotifyMsg::MsgCase::kPlay: 
            return std::make_shared<PlayNetworkEvent>(msg.play());
        case NotifyMsg::MsgCase::kDrawRsp: 
            return std::make_shared<DrawRspNetworkEvent>(msg.drawrsp());
        case NotifyMsg::MsgCase::kUno: 
            return std::make_shared<UnoNetworkEvent>(msg.uno());
        case NotifyMsg::MsgCase::kCatch: 
            return std::make_shared<CatchNetworkEvent>(msg.catch_());
        case NotifyMsg::MsgCase::kCatchRsp: 
            return std::make_shared<CatchRspNetworkEvent>(msg.catchrsp());
        case NotifyMsg::MsgCase::kDoubt: 
            return std::make_shared<DoubtNetworkEvent>(msg.doubt());
        case NotifyMsg::MsgCase::kDoubtRsp: 
            return std::make_shared<DoubtRspNetworkEvent>(msg.doubtrsp());
        default:
            assert(false);
    }
    return nullptr;
}

std::shared_ptr<NetworkEvent> NetworkEvent::Create(const UserOperation &msg)
{
    // client shouldn't receive UserOperation msg
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

MsgTypePtrs UserInputEvent::Process(std::shared_ptr<State> &state)
{
    // default handler for UserInputEvent, do nothing
    // Dummy
    auto msg = std::make_shared<UserOperation>();
    JoinGame *joinGame = msg->mutable_joingame();
    static int x = 1;
    joinGame->set_userid(x);
    x++;
    return {msg};
}

MsgTypePtrs TimerEvent::Process(std::shared_ptr<State> &state)
{
    std::cout << "time ticks" << std::endl;
    return {};
}

// -------------------- NetworkEvent ---------------------
MsgTypePtrs DrawNetworkEvent::Process(std::shared_ptr<State> &state)
{
    // state->mGameState.UpdateAfterDraw();
    // state->mPlayerStates[mDraw.userid()].UpdateAfterDraw(mDraw.number());
    std::cout << "process Draw Event" << std::endl;
    std::cout << mDraw.userid() << " " << mDraw.number() << std::endl;
    return {};
}

MsgTypePtrs SkipNetworkEvent::Process(std::shared_ptr<State> &state)
{
    state->mGameState.UpdateAfterSkip();
    state->mPlayerStates[mSkip.userid()].UpdateAfterSkip();
    return {};
}

MsgTypePtrs PlayNetworkEvent::Process(std::shared_ptr<State> &state)
{
    Card card{mPlay.card()};
    state->mGameState.UpdateAfterPlay(card);
    state->mPlayerStates[mPlay.userid()].UpdateAfterPlay(card);
    return {};
}

MsgTypePtrs DrawRspNetworkEvent::Process(std::shared_ptr<State> &state)
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
    return {};
}

MsgTypePtrs UnoNetworkEvent::Process(std::shared_ptr<State> &state)
{
    return {};
}

MsgTypePtrs CatchNetworkEvent::Process(std::shared_ptr<State> &state)
{
    return {};
}

MsgTypePtrs DoubtNetworkEvent::Process(std::shared_ptr<State> &state)
{
    return {};
}

MsgTypePtrs DoubtRspNetworkEvent::Process(std::shared_ptr<State> &state)
{
    return {};
}

// -------------------- UserInputEvent ---------------------
MsgTypePtrs CursorMoveLeftUserInputEvent::Process(std::shared_ptr<State> &state)
{
    return {};
}

MsgTypePtrs CursorMoveRightUserInputEvent::Process(std::shared_ptr<State> &state)
{    
    return {};
}

MsgTypePtrs PlayUserInputEvent::Process(std::shared_ptr<State> &state)
{    
    return {};
}

MsgTypePtrs PassUserInputEvent::Process(std::shared_ptr<State> &state)
{    
    return {};
}

MsgTypePtrs QuitUserInputEvent::Process(std::shared_ptr<State> &state)
{    
    return {};
}

}}
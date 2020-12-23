#include "../event.h"
#include "state_s.h"

namespace SailGame { namespace Game {

std::shared_ptr<NetworkEvent> NetworkEvent::Create(const NotifyMsg &msg)
{
    assert(false);
    return nullptr;
}

std::shared_ptr<NetworkEvent> NetworkEvent::Create(const UserOperation &msg)
{
    /// TODO: handle msg
    if (msg.has_joingame()) {
        return std::make_shared<JoinGameNetworkEvent>(msg.joingame());
    }
    assert(false);
    return nullptr;
}

std::shared_ptr<UserInputEvent> UserInputEvent::Create(char ch)
{
    assert(false);
    return nullptr;
}

MsgTypePtrs UserInputEvent::Process(std::shared_ptr<State> &state)
{
    assert(false);
    return {};
}

MsgTypePtrs TimerEvent::Process(std::shared_ptr<State> &state)
{
    assert(false);
    return {};
}

static MsgTypePtrs CreateMsgTypePtrs(int curPlayerIndex,
    const MsgTypePtr &rspMsg, const MsgTypePtr &broadcastMsg)
{
    MsgTypePtrs msgTypePtrs;
    for (int i = 0; i < Config::mPlayerNum; i++) {
        msgTypePtrs.push_back(i == curPlayerIndex ? rspMsg : broadcastMsg);
    }
    return msgTypePtrs;
}

// -------------------- NetworkEvent ---------------------
MsgTypePtrs DrawNetworkEvent::Process(std::shared_ptr<State> &state)
{
    // state->mGameState.UpdateAfterDraw();
    // state->mPlayerStates[mDraw.userid()].UpdateAfterDraw(mDraw.number());
    std::cout << mDraw.userid() << " " << mDraw.number() << std::endl;
    return {};
}

MsgTypePtrs SkipNetworkEvent::Process(std::shared_ptr<State> &state)
{
    return {};
}

MsgTypePtrs PlayNetworkEvent::Process(std::shared_ptr<State> &state)
{
    return {};
}

MsgTypePtrs DrawRspNetworkEvent::Process(std::shared_ptr<State> &state)
{
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

MsgTypePtrs JoinGameNetworkEvent::Process(std::shared_ptr<State> &state)
{
    // Dummy
    std::cout << "process JoinGame Event" << std::endl;
    auto msg = std::make_shared<NotifyMsg>();
    Draw *draw = msg->mutable_draw();
    draw->set_userid(mJoinGame.userid());
    draw->set_number(16);
    return CreateMsgTypePtrs(1, msg, msg);
}

// -------------------- UserInputEvent ---------------------
MsgTypePtrs CursorMoveLeftUserInputEvent::Process(std::shared_ptr<State> &state)
{
    assert(false);
    return {};
}

MsgTypePtrs CursorMoveRightUserInputEvent::Process(std::shared_ptr<State> &state)
{    
    assert(false);
    return {};
}

MsgTypePtrs PlayUserInputEvent::Process(std::shared_ptr<State> &state)
{    
    assert(false);
    return {};
}

MsgTypePtrs PassUserInputEvent::Process(std::shared_ptr<State> &state)
{    
    assert(false);
    return {};
}

MsgTypePtrs QuitUserInputEvent::Process(std::shared_ptr<State> &state)
{    
    assert(false);
    return {};
}

}}
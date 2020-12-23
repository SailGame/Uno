#pragma once

#include <iostream>
#include <memory>

#include "hello.pb.h"
#include "../common/types.h"

namespace SailGame { namespace Game {

using ::Uno::UserOperation;
using ::Uno::NotifyMsg;
using ::Uno::Draw;
using ::Uno::Skip;
using ::Uno::Play;
using ::Uno::DrawRsp;
using ::Uno::Uno;
using ::Uno::Catch;
using ::Uno::Doubt;
using ::Uno::DoubtRsp;
using ::Uno::JoinGame;
using Common::UserOperationPtr;
using Common::NotifyMsgPtr;
using Common::MsgTypePtr;
using Common::MsgTypePtrs;

struct State;

enum class EventType {
    Network, UserInput, Timer,
    DrawNetwork, SkipNetwork, PlayNetwork, DrawRspNetwork,
    UnoNetwork, CatchNetwork, CatchRspNetwork,
    DoubtNetwork, DoubtRspNetwork,
    JoinGameNetwork
};

class Event {
public:
    Event(EventType type) : mType(type) {}

    virtual MsgTypePtrs Process(std::shared_ptr<State> &state) = 0;

public:
    EventType mType;
};

class NetworkEvent : public Event {
public:
    NetworkEvent(EventType type) : Event(type) {}

    static std::shared_ptr<NetworkEvent> Create(const NotifyMsg &msg);
    
    static std::shared_ptr<NetworkEvent> Create(const UserOperation &msg);

    virtual MsgTypePtrs Process(std::shared_ptr<State> &state) override = 0;
};

class UserInputEvent : public Event {
public:
    static std::shared_ptr<UserInputEvent> Create(char ch);

    UserInputEvent() : Event(EventType::UserInput) {}

    virtual MsgTypePtrs Process(std::shared_ptr<State> &state) override;
};

class TimerEvent : public Event {
public:
    TimerEvent() : Event(EventType::Timer) {}

    virtual MsgTypePtrs Process(std::shared_ptr<State> &state) override;
};

// -------------------- NetworkEvent ---------------------
class DrawNetworkEvent : public NetworkEvent {
public:
    DrawNetworkEvent(const Draw &draw) : NetworkEvent(EventType::DrawNetwork), mDraw(draw) {}

    virtual MsgTypePtrs Process(std::shared_ptr<State> &state) override;

private:
    Draw mDraw;
};

class SkipNetworkEvent : public NetworkEvent {
public:
    SkipNetworkEvent(const Skip &skip) : NetworkEvent(EventType::SkipNetwork), mSkip(skip) {}

    virtual MsgTypePtrs Process(std::shared_ptr<State> &state) override;

private:
    Skip mSkip;
};

class PlayNetworkEvent : public NetworkEvent {
public:
    PlayNetworkEvent(const Play &play) : NetworkEvent(EventType::PlayNetwork), mPlay(play) {}

    virtual MsgTypePtrs Process(std::shared_ptr<State> &state) override;

private:
    Play mPlay;
};

class DrawRspNetworkEvent : public NetworkEvent {
public:
    DrawRspNetworkEvent(const DrawRsp &drawRsp) 
        : NetworkEvent(EventType::DrawRspNetwork), mDrawRsp(drawRsp) {}

    virtual MsgTypePtrs Process(std::shared_ptr<State> &state) override;

private:
    DrawRsp mDrawRsp;
};

class UnoNetworkEvent : public NetworkEvent {
public:
    UnoNetworkEvent(const Uno &uno) : NetworkEvent(EventType::UnoNetwork), mUno(uno) {}

    virtual MsgTypePtrs Process(std::shared_ptr<State> &state) override;

private:
    Uno mUno;
};

class CatchNetworkEvent : public NetworkEvent {
public:
    CatchNetworkEvent(const Catch &catch_) 
        : NetworkEvent(EventType::CatchNetwork), mCatch(catch_) {}

    virtual MsgTypePtrs Process(std::shared_ptr<State> &state) override;

private:
    Catch mCatch;
};

class DoubtNetworkEvent : public NetworkEvent {
public:
    DoubtNetworkEvent(const Doubt &doubt) 
        : NetworkEvent(EventType::DoubtNetwork), mDoubt(doubt) {}

    virtual MsgTypePtrs Process(std::shared_ptr<State> &state) override;

private:
    Doubt mDoubt;
};

class DoubtRspNetworkEvent : public NetworkEvent {
public:
    DoubtRspNetworkEvent(const DoubtRsp &doubtRsp) 
        : NetworkEvent(EventType::DoubtRspNetwork), mDoubtRsp(doubtRsp) {}

    virtual MsgTypePtrs Process(std::shared_ptr<State> &state) override;

private:
    DoubtRsp mDoubtRsp;
};

class JoinGameNetworkEvent : public NetworkEvent {
public:
    JoinGameNetworkEvent(const JoinGame &joinGame) 
        : NetworkEvent(EventType::JoinGameNetwork), mJoinGame(joinGame) {}

    virtual MsgTypePtrs Process(std::shared_ptr<State> &state) override;

private:
    JoinGame mJoinGame;
};

// -------------------- UserInputEvent ---------------------
class CursorMoveLeftUserInputEvent : public UserInputEvent {
public:
    CursorMoveLeftUserInputEvent() = default;

    virtual MsgTypePtrs Process(std::shared_ptr<State> &state) override;
};

class CursorMoveRightUserInputEvent : public UserInputEvent {
public:
    CursorMoveRightUserInputEvent() = default;

    virtual MsgTypePtrs Process(std::shared_ptr<State> &state) override;
};

class PlayUserInputEvent : public UserInputEvent {
public:
    PlayUserInputEvent() = default;

    virtual MsgTypePtrs Process(std::shared_ptr<State> &state) override;
};

class PassUserInputEvent : public UserInputEvent {
public:
    PassUserInputEvent() = default;

    virtual MsgTypePtrs Process(std::shared_ptr<State> &state) override;
};

class QuitUserInputEvent : public UserInputEvent {
public:
    QuitUserInputEvent() = default;

    virtual MsgTypePtrs Process(std::shared_ptr<State> &state) override;
};

}}
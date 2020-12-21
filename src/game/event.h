#pragma once

#include <iostream>
#include <memory>

#include "hello.pb.h"
#include "state.h"

namespace SailGame { namespace Game {

using ::Uno::NotifyMsg;
using ::Uno::Draw;
using ::Uno::Skip;
using ::Uno::Play;
using ::Uno::DrawRsp;
using ::Uno::Uno;
using ::Uno::Catch;
using ::Uno::Doubt;
using ::Uno::DoubtRsp;

class Event {
public:
    virtual void Process(std::shared_ptr<State> &state) = 0;
};

class NetworkEvent : public Event {
public:
    static std::shared_ptr<NetworkEvent> Create(const NotifyMsg &msg);

    virtual void Process(std::shared_ptr<State> &state) override = 0;
};

class UserInputEvent : public Event {
public:
    static std::shared_ptr<UserInputEvent> Create(char ch);

    UserInputEvent() = default;

    virtual void Process(std::shared_ptr<State> &state) override;
};

class TimerEvent : public Event {
public:
    TimerEvent() = default;

    virtual void Process(std::shared_ptr<State> &state) override;
};

// -------------------- NetworkEvent ---------------------
class DrawNetworkEvent : public NetworkEvent {
public:
    DrawNetworkEvent(const Draw &draw) : mDraw(draw) {}

    virtual void Process(std::shared_ptr<State> &state) override;

private:
    Draw mDraw;
};

class SkipNetworkEvent : public NetworkEvent {
public:
    SkipNetworkEvent(const Skip &skip) : mSkip(skip) {}

    virtual void Process(std::shared_ptr<State> &state) override;

private:
    Skip mSkip;
};

class PlayNetworkEvent : public NetworkEvent {
public:
    PlayNetworkEvent(const Play &play) : mPlay(play) {}

    virtual void Process(std::shared_ptr<State> &state) override;

private:
    Play mPlay;
};

class DrawRspNetworkEvent : public NetworkEvent {
public:
    DrawRspNetworkEvent(const DrawRsp &drawRsp) : mDrawRsp(drawRsp) {}

    virtual void Process(std::shared_ptr<State> &state) override;

private:
    DrawRsp mDrawRsp;
};

class UnoNetworkEvent : public NetworkEvent {
public:
    UnoNetworkEvent(const Uno &uno) : mUno(uno) {}

    virtual void Process(std::shared_ptr<State> &state) override;

private:
    Uno mUno;
};

class CatchNetworkEvent : public NetworkEvent {
public:
    CatchNetworkEvent(const Catch &catch_) : mCatch(catch_) {}

    virtual void Process(std::shared_ptr<State> &state) override;

private:
    Catch mCatch;
};

class DoubtNetworkEvent : public NetworkEvent {
public:
    DoubtNetworkEvent(const Doubt &doubt) : mDoubt(doubt) {}

    virtual void Process(std::shared_ptr<State> &state) override;

private:
    Doubt mDoubt;
};

class DoubtRspNetworkEvent : public NetworkEvent {
public:
    DoubtRspNetworkEvent(const DoubtRsp &doubtRsp) : mDoubtRsp(doubtRsp) {}

    virtual void Process(std::shared_ptr<State> &state) override;

private:
    DoubtRsp mDoubtRsp;
};

// -------------------- UserInputEvent ---------------------
class CursorMoveLeftUserInputEvent : public UserInputEvent {
public:
    CursorMoveLeftUserInputEvent() = default;

    virtual void Process(std::shared_ptr<State> &state) override;
};

class CursorMoveRightUserInputEvent : public UserInputEvent {
public:
    CursorMoveRightUserInputEvent() = default;

    virtual void Process(std::shared_ptr<State> &state) override;
};

class PlayUserInputEvent : public UserInputEvent {
public:
    PlayUserInputEvent() = default;

    virtual void Process(std::shared_ptr<State> &state) override;
};

class PassUserInputEvent : public UserInputEvent {
public:
    PassUserInputEvent() = default;

    virtual void Process(std::shared_ptr<State> &state) override;
};

class QuitUserInputEvent : public UserInputEvent {
public:
    QuitUserInputEvent() = default;

    virtual void Process(std::shared_ptr<State> &state) override;
};

}}
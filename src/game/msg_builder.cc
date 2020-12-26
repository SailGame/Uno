#include "msg_builder.h"

namespace SailGame { namespace Game {

ProviderMsgPtr MsgBuilder::CreateRegisterArgs(int seqId, const std::string &id,
    const std::string gameName, int maxUsers, int minUsers)
{
    ProviderMsg msg;
    msg.set_sequenceid(seqId);

    auto registerArgs = msg.mutable_registerargs();
    registerArgs->set_id(id);
    registerArgs->set_gamename(gameName);

    auto gameSettings = registerArgs->mutable_gamesetting();
    gameSettings->set_maxusers(maxUsers);
    gameSettings->set_minusers(minUsers);

    return std::make_shared<ProviderMsg>(msg);
}

ProviderMsgPtr MsgBuilder::CreateNotifyMsgArgs(int seqId, ErrorNumber err, 
    int roomId, int userId, NotifyMsg custom) 
{
    ProviderMsg msg;
    msg.set_sequenceid(seqId);

    auto args = msg.mutable_notifymsgargs();
    args->set_err(err);
    args->set_roomid(roomId);
    args->set_userid(userId);
    args->mutable_custom()->PackFrom(custom);

    return std::make_shared<ProviderMsg>(msg);
}

NotifyMsg MsgBuilder::CreateGameStart(const InitHandcardsT &initHandcards, 
    Card flippedCard, int firstPlayer) 
{
    NotifyMsg notifyMsg;
    auto gameStart = notifyMsg.mutable_gamestart();
    for (auto i = 0; i < 7; i++) {
        gameStart->add_inithandcards()->CopyFrom(
            initHandcards[i].ConvertToGrpcCard());
    }
    gameStart->mutable_flippedcard()->CopyFrom(
        flippedCard.ConvertToGrpcCard());
    gameStart->set_firstplayerid(firstPlayer);
    return notifyMsg;
}

NotifyMsg MsgBuilder::CreateDraw(int number)
{
    NotifyMsg msg;
    auto draw = msg.mutable_draw();
    draw->set_number(number);
    return msg;
}

NotifyMsg MsgBuilder::CreateDrawRsp(const std::vector<Card> &cards)
{
    NotifyMsg msg;
    auto drawRsp = msg.mutable_drawrsp();
    for (auto card : cards) {
        drawRsp->add_cards()->CopyFrom(card.ConvertToGrpcCard());
    }
    return msg;
}
}}
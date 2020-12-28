#pragma once

#include <core/provider.pb.h>
#include <core/error.pb.h>
#include <uno/uno.pb.h>

#include "card.h"
#include "../common/types.h"

namespace SailGame { namespace Game {

using ::Uno::GameStart;
using ::Uno::NotifyMsg;
using ::Uno::Draw;
using ::Uno::Skip;
using ::Uno::Play;
using Core::ErrorNumber;
using Core::NotifyMsgArgs;
using Core::ProviderMsg;
using Common::ProviderMsgPtr;

class MsgBuilder {
public:
    static ProviderMsgPtr CreateRegisterArgs(int seqId, const std::string &id,
        const std::string &gameName, int maxUsers, int minUsers);

    static ProviderMsgPtr CreateRegisterRet(int seqId, ErrorNumber err);

    static ProviderMsgPtr CreateNotifyMsgArgs(int seqId, ErrorNumber err, 
        int roomId, int userId, const NotifyMsg &custom);

    static NotifyMsg CreateGameStart(const InitHandcardsT &initHandcards, 
        Card flippedCard, int firstPlayer);

    static NotifyMsg CreateDraw(const Draw &draw);

    static NotifyMsg CreateSkip(const Skip &skip);
  
    static NotifyMsg CreatePlay(const Play &play);
  
    static NotifyMsg CreateDrawRsp(const std::vector<Card> &cards);
};
}}
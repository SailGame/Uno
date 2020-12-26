#pragma once

#include <core/provider.pb.h>
#include <core/error.pb.h>
#include <uno/uno.pb.h>

#include "card.h"
#include "../common/types.h"

namespace SailGame { namespace Game {

using ::Uno::GameStart;
using ::Uno::NotifyMsg;
using Core::ErrorNumber;
using Core::NotifyMsgArgs;
using Core::ProviderMsg;
using Common::ProviderMsgPtr;

class MsgBuilder {
public:
    static ProviderMsgPtr CreateRegisterArgs(int seqId, const std::string &id,
        const std::string gameName, int maxUsers, int minUsers);

    static ProviderMsgPtr CreateNotifyMsgArgs(int seqId, ErrorNumber err, 
        int roomId, int userId, NotifyMsg custom);

    static NotifyMsg CreateGameStart(const InitHandcardsT &initHandcards, 
        Card flippedCard, int firstPlayer);

    static NotifyMsg CreateDraw(int number);

    static NotifyMsg CreateSkip();
  
    static NotifyMsg CreateDrawRsp(const std::vector<Card> &cards);
};
}}
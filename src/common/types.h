#pragma once

#include <variant>
#include <memory>

namespace Uno {
struct UserOperation;
struct NotifyMsg;
}

namespace SailGame { namespace Common {

using UserOperationPtr = std::shared_ptr<Uno::UserOperation>;
using NotifyMsgPtr = std::shared_ptr<Uno::NotifyMsg>;
using MsgTypePtr = std::variant<UserOperationPtr, NotifyMsgPtr>;
using MsgTypePtrs = std::vector<MsgTypePtr>;

}}
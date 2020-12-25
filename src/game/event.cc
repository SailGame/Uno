// #include "event.h"
// #include "state.h"

// namespace SailGame { namespace Game {

// std::shared_ptr<NetworkEvent> NetworkEvent::Create(const NotifyMsg &msg)
// {
//     // server shouldn't receive NotifyMsg msg
//     assert(false);
//     return nullptr;
// }

// std::shared_ptr<NetworkEvent> NetworkEvent::Create(const UserOperation &msg)
// {
//     switch (msg.Operation_case()) {
//         case UserOperation::OperationCase::kDraw:
//             return std::make_shared<DrawNetworkEvent>(msg.draw());
//         case UserOperation::OperationCase::kSkip: 
//             return std::make_shared<SkipNetworkEvent>(msg.skip());
//         case UserOperation::OperationCase::kPlay: 
//             return std::make_shared<PlayNetworkEvent>(msg.play());
//         case UserOperation::OperationCase::kUno: 
//             return std::make_shared<UnoNetworkEvent>(msg.uno());
//         case UserOperation::OperationCase::kCatch: 
//             return std::make_shared<CatchNetworkEvent>(msg.catch_());
//         case UserOperation::OperationCase::kDoubt: 
//             return std::make_shared<DoubtNetworkEvent>(msg.doubt());
//         case UserOperation::OperationCase::kJoinGame:
//         case UserOperation::OperationCase::kExit:
//             /// TODO: handle these two kinds of msg
//             return nullptr;
//         default:
//             assert(false);
//     }
//     return nullptr;
// }

// std::shared_ptr<UserInputEvent> UserInputEvent::Create(char ch)
// {
//     assert(false);
//     return nullptr;
// }

// NotifyMsgPtrs UserInputEvent::Process(std::shared_ptr<State> &state)
// {
//     assert(false);
//     return {};
// }

// NotifyMsgPtrs TimerEvent::Process(std::shared_ptr<State> &state)
// {
//     assert(false);
//     return {};
// }

// static NotifyMsgPtrs CreateMsgTypePtrs(int curPlayerIndex,
//     const MsgTypePtr &rspMsg, const MsgTypePtr &broadcastMsg)
// {
//     NotifyMsgPtrs msgTypePtrs;
//     for (int i = 0; i < Config::mPlayerNum; i++) {
//         msgTypePtrs.push_back(i == curPlayerIndex ? rspMsg : broadcastMsg);
//     }
//     return msgTypePtrs;
// }

// // -------------------- NetworkEvent ---------------------
// NotifyMsgPtrs DrawNetworkEvent::Process(std::shared_ptr<State> &state)
// {
//     // state->mGameState.UpdateAfterDraw();
//     // state->mPlayerStates[mDraw.userid()].UpdateAfterDraw(mDraw.number());
//     std::cout << mDraw.userid() << " " << mDraw.number() << std::endl;
//     return {};
// }

// NotifyMsgPtrs SkipNetworkEvent::Process(std::shared_ptr<State> &state)
// {
//     return {};
// }

// NotifyMsgPtrs PlayNetworkEvent::Process(std::shared_ptr<State> &state)
// {
//     return {};
// }

// NotifyMsgPtrs DrawRspNetworkEvent::Process(std::shared_ptr<State> &state)
// {
//     return {};
// }

// NotifyMsgPtrs UnoNetworkEvent::Process(std::shared_ptr<State> &state)
// {
//     return {};
// }

// NotifyMsgPtrs CatchNetworkEvent::Process(std::shared_ptr<State> &state)
// {
//     return {};
// }

// NotifyMsgPtrs CatchRspNetworkEvent::Process(std::shared_ptr<State> &state)
// {
//     return {};
// }

// NotifyMsgPtrs DoubtNetworkEvent::Process(std::shared_ptr<State> &state)
// {
//     return {};
// }

// NotifyMsgPtrs DoubtRspNetworkEvent::Process(std::shared_ptr<State> &state)
// {
//     return {};
// }

// NotifyMsgPtrs JoinGameNetworkEvent::Process(std::shared_ptr<State> &state)
// {
//     // Dummy
//     std::cout << "process JoinGame Event" << std::endl;
//     auto msg = std::make_shared<NotifyMsg>();
//     Draw *draw = msg->mutable_draw();
//     draw->set_userid(mJoinGame.userid());
//     draw->set_number(16);
//     return CreateMsgTypePtrs(1, msg, msg);
// }

// // -------------------- UserInputEvent ---------------------
// NotifyMsgPtrs CursorMoveLeftUserInputEvent::Process(std::shared_ptr<State> &state)
// {
//     assert(false);
//     return {};
// }

// NotifyMsgPtrs CursorMoveRightUserInputEvent::Process(std::shared_ptr<State> &state)
// {    
//     assert(false);
//     return {};
// }

// NotifyMsgPtrs PlayUserInputEvent::Process(std::shared_ptr<State> &state)
// {    
//     assert(false);
//     return {};
// }

// NotifyMsgPtrs PassUserInputEvent::Process(std::shared_ptr<State> &state)
// {    
//     assert(false);
//     return {};
// }

// NotifyMsgPtrs QuitUserInputEvent::Process(std::shared_ptr<State> &state)
// {    
//     assert(false);
//     return {};
// }

// }}
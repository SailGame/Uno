#pragma once

#include <functional>
#include <memory>
#include <queue>
#include <mutex>

#include <core/provider.pb.h>

#include "types.h"

namespace SailGame { namespace Common {

using Core::ProviderMsg;

class EventLoop {
public:
    using OnEventProcessedT = std::function<void(const ProviderMsgPtr &)>;

public:
    EventLoop(const OnEventProcessedT& callback)
        : OnEventProcessed(callback) {}

    void StartLoop() {
        while (true) {
            if (!mEventQueue.empty()) {
                // std::cout << "[EventLoop] Process Event: " << int(mEventQueue.front()->mType) << std::endl;
                OnEventProcessed(mEventQueue.front());
                mEventQueue.pop();
            }
        }
    }

    void AppendEvent(const ProviderMsgPtr &event) {
        // multiple threads may invoke this at the same time
        std::lock_guard<std::mutex> lock(mMutex);
        // std::cout << "[EventLoop] Append Event: " << int(event->mType) << std::endl;
        mEventQueue.push(event);
    }
    
private:
    OnEventProcessedT OnEventProcessed;

private:
    std::queue<ProviderMsgPtr> mEventQueue;
    std::mutex mMutex;
};
}}
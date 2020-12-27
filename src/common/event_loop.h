#pragma once

#include <functional>
#include <memory>
#include <queue>
#include <mutex>

#include <core/provider.pb.h>

#include "types.h"

namespace SailGame { namespace Common {

using Core::ProviderMsg;

class EventLoopSubscriber {
public:
    virtual void OnEventProcessed(const ProviderMsgPtr &) = 0;
};

class EventLoop {
public:
    EventLoop() = default;

    void StartLoop() {
        while (true) {
            if (!mEventQueue.empty()) {
                // std::cout << "[EventLoop] Process Event: " << int(mEventQueue.front()->mType) << std::endl;
                mSubscriber->OnEventProcessed(mEventQueue.front());
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

    void SetSubscriber(EventLoopSubscriber *subscriber) {
        mSubscriber = subscriber;
    }

private:
    std::queue<ProviderMsgPtr> mEventQueue;
    std::mutex mMutex;
    EventLoopSubscriber *mSubscriber{nullptr};
};
}}
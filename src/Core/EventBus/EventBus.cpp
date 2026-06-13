#include "EventBus.hpp"

namespace Voxel {
    EventBus::EventBus() {}

    EventBus::~EventBus()
    {
        clear();
    }

    void EventBus::dispatchEvents()
    {
        while (!this->_eventQueue.empty()) {
            auto queuedEvent = this->_eventQueue.front();

            this->_cleanupDeadSubscribers(queuedEvent.type);

            auto it = this->_subscribers.find(queuedEvent.type);
            if (it != this->_subscribers.end()) {
                for (auto& wrapper : it->second) {
                    if (!wrapper.subscriber.expired())
                        wrapper.callback(*queuedEvent.event);
                }
            }

            this->_eventQueue.pop();
        }
    }

    void EventBus::clear()
    {
        this->_subscribers.clear();
        while (!this->_eventQueue.empty()) {
            this->_eventQueue.pop();
        }
    }

    void EventBus::_cleanupDeadSubscribers(std::type_index type)
    {
        auto it = this->_subscribers.find(type);
        if (it != this->_subscribers.end()) {
            auto& callbacks = it->second;
            callbacks.erase(
                std::remove_if(callbacks.begin(), callbacks.end(),
                    [](const _CallbackWrapper& wrapper) {
                        return wrapper.subscriber.expired();
                    }),
                callbacks.end()
            );
        }
    }
}

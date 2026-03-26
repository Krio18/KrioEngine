#pragma once

#include "../EventSystem/Event.hpp"
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <queue>
#include <algorithm>

namespace Krio {
    class EventBus {
        public:
            EventBus();
            ~EventBus();

            template<typename EventType>
            std::shared_ptr<void> subscribe(std::function<void(const EventType&)> callback);

            template<typename EventType, typename T>
            std::shared_ptr<void> subscribe(T* instance, void (T::*method)(const EventType&));

            template<typename EventType>
            void publish(const EventType& event);

            template<typename EventType>
            void queueEvent(const EventType& event);

            void dispatchEvents();

            void clear();

        private:
            struct _CallbackWrapper {
                std::weak_ptr<void> subscriber;
                std::function<void(const Event&)> callback;
            };

            std::unordered_map<std::type_index, std::vector<_CallbackWrapper>> _subscribers;

            struct _QueuedEvent {
                std::type_index type;
                std::shared_ptr<Event> event;
            };
            std::queue<_QueuedEvent> _eventQueue;

            void _cleanupDeadSubscribers(std::type_index type);
    };

    template<typename EventType>
    std::shared_ptr<void> EventBus::subscribe(std::function<void(const EventType&)> callback)
    {
        static_assert(std::is_base_of<Event, EventType>::value, "EventType must inherit from Event");

        auto subscriber = std::make_shared<int>(0);
        auto typeIndex = std::type_index(typeid(EventType));

        _CallbackWrapper wrapper;
        wrapper.subscriber = subscriber;
        wrapper.callback = [callback](const Event& e) {
            callback(static_cast<const EventType&>(e));
        };

        this->_subscribers[typeIndex].push_back(wrapper);

        return subscriber;
    }

    template<typename EventType, typename T>
    std::shared_ptr<void> EventBus::subscribe(T* instance, void (T::*method)(const EventType&))
    {
        static_assert(std::is_base_of<Event, EventType>::value, "EventType must inherit from Event");

        auto callback = [instance, method](const EventType& event) {
            (instance->*method)(event);
        };

        return subscribe<EventType>(callback);
    }

    template<typename EventType>
    void EventBus::publish(const EventType& event)
    {
        static_assert(std::is_base_of<Event, EventType>::value, "EventType must inherit from Event");

        auto typeIndex = std::type_index(typeid(EventType));

        this->_cleanupDeadSubscribers(typeIndex);

        auto it = this->_subscribers.find(typeIndex);
        if (it != this->_subscribers.end()) {
            for (auto& wrapper : it->second) {
                if (!wrapper.subscriber.expired())
                    wrapper.callback(event);
            }
        }
    }

    template<typename EventType>
    void EventBus::queueEvent(const EventType& event)
    {
        static_assert(std::is_base_of<Event, EventType>::value, "EventType must inherit from Event");

        auto typeIndex = std::type_index(typeid(EventType));
        auto eventCopy = std::make_shared<EventType>(event);

        this->_eventQueue.push({typeIndex, eventCopy});
    }
}

#pragma once

#include <functional>
#include <array>
#include "Utils.h"

namespace TerrainGenerator {
    enum class EventType {
        WindowClose = 0,

        MouseButtonPressed,
        MouseButtonReleased,

        EventsCount
    };

    enum class ButtonType {
        A = 65, W = 87, S = 83, D = 68
    };


    struct BaseEvent {
    public:
        virtual ~BaseEvent() = default;

        virtual EventType getType() const = 0;
    };

    class EventDispatcher {
        std::array<std::function<void(BaseEvent & )>, static_cast<size_t>(EventType::EventsCount)> eventCallbacks;
    public:
        template<typename EventType>
        void addEventListener(std::function<void(EventType &)> callback) {
            auto baseCallback = [func = std::move(callback)](BaseEvent &e) {
                if (e.getType() == EventType::type) {
                    func(static_cast<EventType &>(e));
                }
            };
            eventCallbacks[static_cast<size_t>(EventType::type)] = std::move(baseCallback);
        }

        void dispatch(BaseEvent &event) {
            auto &callback = eventCallbacks[static_cast<size_t>(event.getType())];
            if (callback) {
                callback(event);
            }
        }
    };

    struct EventWindowClose : public BaseEvent {
        static const EventType type = EventType::WindowClose;

        virtual EventType getType() const override { return type; }
    };

    struct EventMouseButtonPressed : public BaseEvent {
        static const EventType type = EventType::MouseButtonPressed;

        ButtonType key;

        EventMouseButtonPressed(int key) : key(static_cast<ButtonType>(key)) {}

        virtual EventType getType() const override { return type; }
    };
}
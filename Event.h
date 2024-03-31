#pragma once

#include <functional>
#include <array>
#include "Utils.h"

namespace TerrainGenerator {
    enum class EventType {
        WindowResize = 0,
        WindowClose,

        KeyPressed,
        KeyReleased,

        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,

        EventsCount
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

    struct EventMouseMoved : public BaseEvent {
        static const EventType type = EventType::MouseMoved;
        double x;
        double y;

        EventMouseMoved(const double x, const double y) : x(x), y(y) {}

        virtual EventType getType() const override { return type; }
    };

    struct EventWindowResize : public BaseEvent {
        static const EventType type = EventType::WindowResize;
        Uint32 width;
        Uint32 height;

        EventWindowResize(const Uint32 width, const Uint32 height) : width(width), height(height) {}

        virtual EventType getType() const override { return type; }
    };

    struct EventWindowClose : public BaseEvent {
        static const EventType type = EventType::WindowClose;

        virtual EventType getType() const override { return type; }
    };
}
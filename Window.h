#pragma once

#include "Event.h"
#include "Utils.h"

#include <string>
#include <functional>
#include "Utils.h"

struct GLFWwindow;

namespace TerrainGenerator {
    class Window {
    public:
        using EventCallbackFn =
                std::function<void(BaseEvent &)>;

        Window(std::string title, const Uint32 width, const Uint32 height);

        ~Window();

        Window(const Window &) = delete;

        Window(Window &&) = delete;

        Window operator=(const Window &) = delete;

        Window operator=(Window &&) = delete;

        void onUpdate();

        Uint32 getWidth() const { return data_.width; }

        Uint32 getHeight() const { return data_.height; };

        void setEventCallback(const EventCallbackFn &callback) {
            data_.eventCallbackFn = callback;
        }

    private:
        struct WindowData {
            std::string title;
            Uint32 width;
            Uint32 height;
            EventCallbackFn eventCallbackFn;
        };
        GLFWwindow *window_ = nullptr;
        WindowData data_;

        int init();

        void destroy();
    };
}
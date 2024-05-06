#pragma once

#include "Event.h"
#include "Utils.h"

#include <string>
#include <functional>

#include "../terrain_generator/generator.hpp"


struct GLFWwindow;

namespace TerrainGenerator {
    void loadPreview();
    class Window {
    public:
        Terrain terrain;
        using EventCallbackFn =
                std::function<void(BaseEvent &)>;

        Window(std::string title, const Int32 width, const Int32 height);

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

//        void loadPreview();

    private:
        struct WindowData {
            std::string title;
            Int32 width;
            Int32 height;
            EventCallbackFn eventCallbackFn;
        };
        GLFWwindow *window_ = nullptr;
        WindowData data_;

        int init();

        void destroy();
    };
}
#pragma once

#include <memory>
#include "Utils.h"
#include "Event.h"

namespace TerrainGenerator {
    class Application {
        std::unique_ptr<class Window> window_;
        EventDispatcher eventDispatcher_;
        bool closeWindow_ = false;
    public:
        Application();

        virtual ~Application();

        Application(const Application &) = delete;

        Application(Application &&) = delete;

        Application operator=(const Application &) = delete;

        Application operator=(Application &&) = delete;

        virtual int start(Uint32 windowWidth, Uint32 windowHeight, const char *title);

        virtual void onUpdate() {}
    };
}
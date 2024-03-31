#include "Application.h"
#include "Window.h"
#include "Event.h"
#include <iostream>

namespace TerrainGenerator {
    Application::Application() {}

    Application::~Application() {}

    int Application::start(Uint32 windowWidth, Uint32 windowHeight, const char *title) {
        window_ = std::make_unique<Window>(title, windowWidth, windowHeight);

        eventDispatcher_.addEventListener<EventMouseMoved>(
                [](EventMouseMoved &event) {

                });

        eventDispatcher_.addEventListener<EventWindowResize>(
                [](EventWindowResize &event) {
                    std::cout << "resize\n";
                });

        eventDispatcher_.addEventListener<EventWindowClose>(
                [&](EventWindowClose &event) {
                    closeWindow_ = true;
                });

        window_->setEventCallback(
                [&](BaseEvent &event) {
                    eventDispatcher_.dispatch(event);
                }
        );

        while (!closeWindow_) {
            window_->onUpdate();
            onUpdate();
        }
        return 0;
    }
}
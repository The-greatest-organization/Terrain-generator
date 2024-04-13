#include "Application.h"
#include "Window.h"
#include "Event.h"

namespace TerrainGenerator {
    Application::Application() = default;

    Application::~Application() = default;

    int Application::start(Uint32 windowWidth, Uint32 windowHeight, const char *title) {
        window_ = std::make_unique<Window>(title, windowWidth, windowHeight);

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
#include "Application.h"
#include "Window.h"
#include "Event.h"
#include <iostream>
#include <thread>

namespace TerrainGenerator {
    class Terrain;
    static bool load = false;
    static Terrain* terrain;
    Application::Application() = default;

    Application::~Application() = default;

    int Application::start(Uint32 windowWidth, Uint32 windowHeight, const char *title) {
        window_ = std::make_unique<Window>(title, windowWidth, windowHeight);
        //terrain.generate();
        eventDispatcher_.addEventListener<EventWindowClose>(
                [&](EventWindowClose &event) {
                    closeWindow_ = true;
                });

        eventDispatcher_.addEventListener<EventMouseButtonPressed>(
                [&](EventMouseButtonPressed &event) {
                    if (event.key == ButtonType::A)
                        std::cout << "a\n";
                    if (event.key == ButtonType::W)
                        std::cout << "w\n";
                    if (event.key == ButtonType::S)
                        std::cout << "s\n";
                    if (event.key == ButtonType::D)
                        std::cout << "d\n";
                });

        window_->setEventCallback(
                [&](BaseEvent &event) {
                    eventDispatcher_.dispatch(event);
                }
        );

//        isChange = false;
//        loadPreview();

        while (!closeWindow_) {
            window_->onUpdate();
            onUpdate();
            if (load) {
                std::cout << load;
                load = false;
                std::thread th1(loadPreview);
                th1.join();
            }

        }
        return 0;
    }
}
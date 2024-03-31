#include "Window.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include "Utils.h"

namespace TerrainGenerator {

    static bool GLFWInitialized = false;

    Window::Window(std::string title, const Uint32 width, const Uint32 height) : data_(
            {std::move(title), width, height}) {
        init();
    }

    Window::~Window() {
        destroy();
    }

    int Window::init() {
        if (!GLFWInitialized) {
            if (!glfwInit())
                return -1;

            GLFWInitialized = true;
        }
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        std::cout << data_.width << "\n";
        //window_ = glfwCreateWindow(mode->width, mode->height, data_.title_.c_str(), monitor, NULL);
        window_ = glfwCreateWindow(data_.width, data_.height, data_.title.c_str(), NULL, NULL);

        if (!window_) {
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window_);

        glfwSetWindowUserPointer(window_, &data_);

        glfwSetWindowSizeCallback(
                window_,
                [](GLFWwindow *window, int width, int height) {
                    WindowData &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
                    data.width = width;
                    data.height = height;
                    EventWindowResize event(width, height);
                    data.eventCallbackFn(event);
                }
        );

        glfwSetCursorPosCallback(
                window_,
                [](GLFWwindow *window, double x, double y) {
                    WindowData &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
                    EventMouseMoved event(x, y);
                    data.eventCallbackFn(event);
                }
        );
        glfwSetWindowCloseCallback(
                window_,
                [](GLFWwindow *window) {
                    WindowData &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
                    EventWindowClose event;
                    data.eventCallbackFn(event);
                }
        );
        return 0;
    }

    void Window::destroy() {
        glfwDestroyWindow(window_);
        glfwTerminate();
    }

    void Window::onUpdate() {
        glClearColor(1, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

#include "Window.h"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace TerrainGenerator {

    static bool GLFWInitialized = false;
    static Int32 resolution = 0;
    static Int32 size = 0;
    static Int32 seed = 0;
    static const char* biomes[] {"Plain", "Mountains", "Desert", "Sea"};
    static const Int32 countBiomes = 4;
    static Int32 biome = 0;
    static Float32 intensityTerrain = 0;
    static Int32 stepsTerrain = 0;
    static Float32 intensityHydraulic = 0;
    static Int32 stepsHydraulic = 0;


    Window::Window(std::string title, const Int32 width, const Int32 height) : data_(
            {std::move(title), width, height}) {
        init();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(window_, true);
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
        data_.width = mode->width;
        data_.height = mode->height;
        //glfwGetMonitorPhysicalSize(monitor, &data_.width, &data_.height);
        //window_ = glfwCreateWindow(mode->width, mode->height, data_.title.c_str(), monitor, NULL);
        window_ = glfwCreateWindow(data_.width, data_.height, data_.title.c_str(), NULL, NULL);

        if (!window_) {
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window_);

        glfwSetWindowUserPointer(window_, &data_);

        glfwSetWindowSizeCallback(
                window_,
                [](GLFWwindow *window, Int32 width, Int32 height) {
                    WindowData &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
                    data.width = width;
                    data.height = height;
                    EventWindowResize event(width, height);
                    data.eventCallbackFn(event);
                }
        );

        glfwSetCursorPosCallback(
                window_,
                [](GLFWwindow *window, Float64 x, Float64 y) {
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
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window_);
        glfwTerminate();
    }

    void Window::onUpdate() {
        glClearColor(1, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(getWidth());
        io.DisplaySize.y = static_cast<float>(getHeight());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
//        ImGui::ShowDemoWindow();

        ImGui::SetNextWindowPos({0.f,0.f});
        Float32 windowWidth = static_cast<Float32>(getWidth());
        Float32 windowHeight = static_cast<Float32>(getHeight());

        ImGui::SetNextWindowSize({windowWidth,windowHeight});

        ImGui::Begin( "TerrainGenerator", nullptr, ImGuiWindowFlags_NoTitleBar);

        ImGui::BeginChild("Preview", {windowWidth / 4, windowHeight / 4}, true);

        ImGui::Text("Preview");

        ImGui::EndChild();


        ImGui::BeginChild("Settings", {windowWidth / 4, 3 * windowHeight / 4}, true);

        ImGui::SeparatorText("Main settings:");
        ImGui::InputInt("Seed", &seed);
        ImGui::Combo("Biome", &biome, biomes, countBiomes);
        ImGui::SliderInt("Resolution", &resolution,0,100);
        ImGui::SliderInt("Size", &size,0,100);
        ImGui::SeparatorText("Terrain formation:");

        ImGui::SeparatorText("Hydraulic erosion:");

        ImGui::SeparatorText("Coloring:");

        ImGui::EndChild();

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

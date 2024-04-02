#include "Window.h"

#include <random>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include "../terrain_generator/stb_image.h"
#include "../terrain_generator/generator.h"

namespace TerrainGenerator {
    static std::minstd_rand generator;
    static bool GLFWInitialized = false;
    static Int32 resolution = 15;
    static Int32 size = 20;
    static Int32 seed = generator();
    static const char *biomes[]{"Plain", "Mountains", "Desert", "Sea"};
    static const Int32 countBiomes = 4;
    static Int32 biome = 0;
    static Int32 maxHeight = 5000;
    static Float32 intensityTerrain = 1.2;
    static Float32 frequencyTerrain = 4;
    static Float32 amplitudeTerrain = 1;
    static Int32 stepsTerrain = 10;
    static Float32 subIntensityHydraulic = 0.5;
    static Float32 addIntensityHydraulic = 0.4;
    static Int32 stepsHydraulic = 10;
    static Int32 stepsRealHydraulic = 0;
    static Float32 intensityRealHydraulic = 1;
    static Float32 fluidityRealHydraulic = 0.8;
    static Float32 flowabilityRealHydraulic = 2;
    static Float32 evaporationRealHydraulic = 0.5;
    static const char *seasons[]{"Summer", "Autumn", "Winter", "Spring"};
    static const Int32 countSeasons = 4;
    static Int32 season = 0;
    static bool cuda = false;
    static bool run = false;


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
        window_ = glfwCreateWindow(data_.width, data_.height, data_.title.c_str(), NULL, NULL);

        if (!window_) {
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window_);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            return -1;
        }

        glfwSetWindowUserPointer(window_, &data_);

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

    bool LoadTextureFromFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height) {
        int image_width = 0;
        int image_height = 0;
        unsigned char *image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
        if (image_data == NULL)
            return false;

        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        stbi_image_free(image_data);

        *out_texture = image_texture;
        *out_width = image_width;
        *out_height = image_height;

        return true;
    }

    void showPreview(Float32 windowWidth, Float32 windowHeight) {
        ImGui::BeginChild("Preview", {windowWidth / 4, windowHeight * 1.5f / 4}, true);
        ImGui::SeparatorText("Preview:");
        Terrain tr = Terrain(size, maxHeight, seed, resolution, intensityTerrain, frequencyTerrain, amplitudeTerrain,
                             stepsTerrain,
                             stepsHydraulic, subIntensityHydraulic, addIntensityHydraulic, stepsRealHydraulic,
                             intensityRealHydraulic, fluidityRealHydraulic, flowabilityRealHydraulic,
                             evaporationRealHydraulic, cuda);
        tr.generate();
        tr.export_png("preview.png");
        int previewWidth = 0;
        int previewHeight = 0;
        GLuint previewTexture = 0;
        bool ret = LoadTextureFromFile("preview.png", &previewTexture, &previewWidth, &previewHeight);

        if (!ret) {
            ImGui::Text("no image");
        }
        ImGui::Image((void *) (intptr_t) previewTexture, ImVec2(previewWidth, previewHeight));

        ImGui::EndChild();
    }

    void showSettings(Float32 windowWidth, Float32 windowHeight) {
        ImGui::BeginChild("Settings", {windowWidth / 4, windowHeight * 2.5f / 4}, true);

        ImGui::SeparatorText("Main settings:");
        ImGui::InputInt("Seed", &seed);
        ImGui::SliderInt("Resolution", &resolution, 0, 100);
        ImGui::SliderInt("Size", &size, 0, 100);
        ImGui::SeparatorText("Terrain formation:");
        ImGui::SliderFloat("Intensity", &intensityTerrain, 0, 2);
        ImGui::SliderFloat("Frequency", &frequencyTerrain, 0, 10);
        ImGui::SliderFloat("Amplitude", &amplitudeTerrain, 0, 10);
        ImGui::SliderInt("Max height", &maxHeight, 0, 5000);
        ImGui::SliderInt("Steps##1", &stepsTerrain, 0, 100);

        ImGui::SeparatorText("Smoothing hydraulic erosion:");
        ImGui::SliderFloat("Sub intensity", &subIntensityHydraulic, 0, 1);
        ImGui::SliderFloat("Add intensity", &addIntensityHydraulic, 0, 1);
        ImGui::SliderInt("Steps##2", &stepsHydraulic, 0, 100);

        ImGui::SeparatorText("Real hydraulic erosion:");
        ImGui::SliderFloat("Intensity##2", &intensityRealHydraulic, 0, 1);
        ImGui::SliderFloat("Fluidity of water", &fluidityRealHydraulic, 0, 1);
        ImGui::SliderFloat("Soil flowability", &flowabilityRealHydraulic, 0, 10);
        ImGui::SliderFloat("Evaporation of water", &evaporationRealHydraulic, 0, 1);
        ImGui::SliderInt("Steps##3", &stepsRealHydraulic, 0, 100);

        ImGui::SeparatorText("Running:");
        ImGui::Checkbox("Cuda", &cuda);
        if (ImGui::Button("Run", {100.f, 50.f})) {
            run = true;
        }
        ImGui::EndChild();
    }

    void showTerrain(Float32 windowWidth, Float32 windowHeight) {
        ImGui::SetCursorPos({2 * ImGui::GetCursorStartPos().x + windowWidth / 4, ImGui::GetCursorStartPos().y});
        ImGui::BeginChild("Terrain", {3 * windowWidth / 4, 4 * windowHeight / 4 + 3}, true);
        ImGui::Text("Terrain");
        ImGui::EndChild();
    }

    void Window::onUpdate() {
        glClearColor(1, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(getWidth());
        io.DisplaySize.y = static_cast<float>(getHeight());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos({0.f, 0.f});
        Float32 windowWidth = static_cast<Float32>(getWidth());
        Float32 windowHeight = static_cast<Float32>(getHeight());

        ImGui::SetNextWindowSize({windowWidth, windowHeight});
        windowWidth -= ImGui::GetCursorStartPos().x;
        windowHeight -= ImGui::GetCursorStartPos().y;
        ImGui::Begin("TerrainGenerator", nullptr, ImGuiWindowFlags_NoTitleBar);

        showPreview(windowWidth, windowHeight);
        showSettings(windowWidth, windowHeight);
        showTerrain(windowWidth, windowHeight);

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

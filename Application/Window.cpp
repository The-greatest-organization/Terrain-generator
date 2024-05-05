#include "Window.h"

#include <random>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "../terrain_generator/stb_image.h"
#include "../terrain_generator/generator.hpp"


namespace TerrainGenerator {
    static std::minstd_rand generator;
    static bool GLFWInitialized = false;
    Terrain terrain;
//    static Int32 resolution = 1;
//    static Int32 size = 400;
//    static Int32 seed = generator();
//    static const char *biomes[]{"Plain", "Mountains", "Desert", "Sea"};
//    static const Int32 countBiomes = 4;
//    static Int32 biome = 0;
//    static Int32 maxHeight = 2000;
//    static Float32 intensityTerrain = 0.9;
//    static Float32 frequencyTerrain = 4;
//    static Float32 amplitudeTerrain = 1;
//    static Int32 stepsTerrain = 12;
//    static Float32 subIntensityHydraulic = 0.5;
//    static Float32 addIntensityHydraulic = 0.4;
//    static Int32 stepsHydraulic = 10;
//    static Int32 stepsRealHydraulic = 10;
//    static Float32 intensityRealHydraulic = 0.6;
//    static Float32 fluidityRealHydraulic = 0.5;
//    static Float32 flowabilityRealHydraulic = 1;
//    static Float32 evaporationRealHydraulic = 0.5;
//    static const char *seasons[]{"Summer", "Autumn", "Winter", "Spring"};
//    static const Int32 countSeasons = 4;
//    static Int32 season = 0;
//    static bool cuda = false;
    static bool run = false;

    static Int32 maxSize = 500;
    static Int32 maxResolution = 10;

    int previewWidth = 0;
    int previewHeight = 0;
    GLuint previewTexture = 0;
    bool retLoadPreview = false;

    int previewUpdateWidth = 0;
    int previewUpdateHeight = 0;
    GLuint previewUpdateTexture = 0;

    bool isChange = true;

    Window::Window(std::string title, const Int32 width, const Int32 height)
            : data_({std::move(title), width, height}) {
        init();
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(window_, true);
    }

    Window::~Window() { destroy(); }

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
        window_ = glfwCreateWindow(data_.width, data_.height, data_.title.c_str(),
                                   NULL, NULL);

        if (!window_) {
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window_);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            return -1;
        }

        glfwSetWindowUserPointer(window_, &data_);

        glfwSetKeyCallback(window_, [](GLFWwindow *window, int key, int scancode, int action, int mods){
            if (action != GLFW_PRESS) return;
            WindowData &data =
                    *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
            EventMouseButtonPressed event = EventMouseButtonPressed(key);
            data.eventCallbackFn(event);
        });

        glfwSetWindowCloseCallback(window_, [](GLFWwindow *window) {
            WindowData &data =
                    *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
            EventWindowClose event;
            data.eventCallbackFn(event);
        });

        return 0;
    }

    void Window::destroy() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(window_);
        glfwTerminate();
    }

    bool LoadTextureFromFile(const char *filename, GLuint *out_texture,
                             int *out_width, int *out_height) {
        int image_width = 0;
        int image_height = 0;
        unsigned char *image_data =
                stbi_load(filename, &image_width, &image_height, NULL, 4);
        if (!image_data)
            return false;

        GLuint image_texture;
        glGenTextures(1, &image_texture);
        glBindTexture(GL_TEXTURE_2D, image_texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, image_data);
        stbi_image_free(image_data);

        *out_texture = image_texture;
        *out_width = image_width;
        *out_height = image_height;

        return true;
    }


    void loadPreview() {
        terrain.generate()

        /// TODO: get the pixels from generator directly to avoid working with files

        terrain.export_png("preview.png");
        retLoadPreview = LoadTextureFromFile("preview.png", &previewTexture,
                                             &previewWidth, &previewHeight);

    }

    void showPreview(Float32 windowWidth, Float32 windowHeight) {
        ImGui::BeginChild("Preview", {windowWidth / 4, windowHeight * 1.5f / 4},
                          true);
        ImGui::SeparatorText("Preview:");
        if (isChange) {
            float posY = ImGui::GetCursorPosY() + windowHeight * 1.5f / 10;
            float posX = ImGui::GetCursorPosX() + windowWidth / 12;
            ImGui::SetCursorPos({posX, posY});
            ImGui::Text("Preview was modified");
            ImGui::SetCursorPos({posX + 20, ImGui::GetCursorPosY() + 10});
            if (ImGui::Button("Update", {100, 50})) {
                isChange = false;
                loadPreview();
            }
        } else {
            if (!retLoadPreview) {
                float posY = ImGui::GetCursorPosY() + windowHeight * 1.5f / 10;
                float posX = ImGui::GetCursorPosX() + windowWidth / 12;
                ImGui::SetCursorPos({posX, posY});
                ImGui::Text("no image");
            } else {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
                ImGui::Image((void *) (intptr_t) previewTexture,
                             ImVec2(previewWidth, previewHeight));
            }
        }
        ImGui::EndChild();
    }

    void showSettings(Float32 windowWidth, Float32 windowHeight) {
        ImGui::BeginChild("Settings", {windowWidth / 4, windowHeight * 2.5f / 4},
                          true);

        ImGui::SeparatorText("Main settings:");
        float posY = ImGui::GetCursorPosY();
        isChange = ImGui::InputInt("Seed", &terrain.params.seed) || isChange;
        float posX = ImGui::GetCursorPosX();
        ImGui::SetCursorPos({posX + windowWidth * 3 / 16, posY});
        if  (ImGui::Button("Random", {20, 30})) {
            seed = generator();
            isChange = true;
        }
        ImGui::SetCursorPosX(posX);
        ImGui::SliderInt("Resolution", &terrain.params.resolution, 1, maxResolution);
        ImGui::SliderInt("Size", &size, 1, maxSize);
        ImGui::SeparatorText("Terrain formation:");
        isChange = ImGui::SliderFloat("Intensity", &terrain.params.perlin_intensity, 0, 1) || isChange;
        isChange = ImGui::SliderFloat("Frequency", &terrain.params.perlin_frequency, 0, 5) || isChange;
        isChange = ImGui::SliderFloat("Amplitude", &terrain.params.perlin_amplitude, 0, 5) || isChange;
        isChange = ImGui::SliderInt("Max height", &terrain.params.max_height, 0, 19000) || isChange;
        isChange = ImGui::SliderInt("Steps##1", &terrain.params.perlin_iterations, 0, 20) || isChange;

        ImGui::SeparatorText("Smoothing hydraulic erosion:");
        isChange = ImGui::SliderFloat("Sub intensity", &terrain.params.smoothing_hydraulic_erosion_sub_intensity, 0, 1) || isChange;
        isChange = ImGui::SliderFloat("Add intensity", &terrain.params.smoothing_hydraulic_erosion_add_intensity, 0, subIntensityHydraulic) || isChange;
        isChange = ImGui::SliderInt("Steps##2", &terrain.params.real_hydraulic_erosion_steps, 0, 100) || isChange;

        ImGui::SeparatorText("Real hydraulic erosion:");
        isChange = ImGui::SliderFloat("Intensity##2", &terrain.params.real_hydraulic_erosion_intensity, 0, 15) || isChange;
        isChange = ImGui::SliderFloat("Fluidity of water", &terrain.params.real_hydraulic_erosion_fluidity_of_water, 0, 1) || isChange;
        isChange = ImGui::SliderFloat("Soil flowability", &terrain.params.real_hydraulic_erosion_soil_flowability, 0, 2) || isChange;
        isChange = ImGui::SliderFloat("Evaporation of water", &terrain.params.real_hydraulic_erosion_non_evaporation_of_water, 0, 1) || isChange;
        isChange = ImGui::SliderInt("Steps##3", &terrain.params.real_hydraulic_erosion_steps, 0, 100) || isChange;

        ImGui::SeparatorText("Running:");
        isChange = ImGui::Checkbox("Cuda", &terrain.params.cuda) || isChange;
        if (ImGui::Button("Run", {100.f, 50.f})) {
            run = true;
        }
        ImGui::EndChild();
    }

    void showTerrain(Float32 windowWidth, Float32 windowHeight) {
        ImGui::SetCursorPos({2 * ImGui::GetCursorStartPos().x + windowWidth / 4,
                             ImGui::GetCursorStartPos().y});
        ImGui::BeginChild("Terrain", {3 * windowWidth / 4, 4 * windowHeight / 4 + 3},
                          true);
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

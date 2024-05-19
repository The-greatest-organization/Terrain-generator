#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Tiny3D/Render.hpp"

#include <cassert>
#include <vector>

tiny3d::PerspectiveCamera3D camera;

void process_input(GLFWwindow* window, float delta) {
    glfwSetWindowShouldClose(window, glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.Position() += glm::vec4(0.0f, 0.0, -1.0, 0.0) * delta;
    }
    
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.Position() += glm::vec4(0.0f, 0.0, 1.0, 0.0) * delta;
    }
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.Position() += glm::vec4(-1.0f, 0.0, 0.0, 0.0) * delta;
    }
    
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.Position() += glm::vec4(1.0f, 0.0, 0.0, 0.0) * delta;
    }
}

int main(int argc, char** argv) {
    if (glfwInit() == -1) {
        assert(0 && "Failed to initialize GLFW infrastructure");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Terrain generator", glfwGetPrimaryMonitor(), nullptr);

    if (window == nullptr) {
        glfwTerminate();
        assert(0 && "Failed to create GLFW window");
        return 1;
    }

    glfwMaximizeWindow(window);

    glfwMakeContextCurrent(window);
    glClearColor(.4f, .4f, .4f, 1);

    // Assuming that size of window is static
    // When we'll render to texture it's not so important

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    std::vector<float> plane {
        -0.5, 0, -0.5, // position
        0.5, 0, -0.5, // position
        -0.5, 0, 0.5, // position
        0.5, 0, 0.5, // position

        0, 1, 0, // normal
        0, 1, 0, // normal
        0, 1, 0, // normal
        0, 1, 0, // normal
    };

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        process_input(window, 0.0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

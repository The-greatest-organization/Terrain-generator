#include <GLFW/glfw3.h>
#include <cassert>

int main(int argc, char** argv) {
    if (glfwInit() == -1) {
        assert(0 && "Failed to initialize GLFW infrastructure");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
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
    glClearColor(1, 1, 0, 1);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
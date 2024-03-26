#include <GLFW/glfw3.h>
#include <cassert>

int main(int argc, char** argv) {
    int code = glfwInit();

    if (code == -1) {
        return -1;
    }   

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Terrain generator", glfwGetPrimaryMonitor(), NULL);

    if (window == nullptr) {
        glfwTerminate();
        return 1;
    }

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
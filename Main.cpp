#include <GLFW/glfw3.h>

int main(int argc, char** argv) {
    if (!glfwInit()) {
        return -1;
    }
    
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Terrain generator", glfwGetPrimaryMonitor(), NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glClearColor(1, 1, 0, 1);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
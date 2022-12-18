// Include the necessary OpenGL headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Include the standard C++ headers
#include <cstring>
#include <vector>
#include <iostream>

// Include the project headers
#include "model.h"
#include "GPU.h"


const int WIDTH = 640;
const int HEIGHT = 480;
glm::vec3 position(0.0f, 0.0f, 4.f);
glm::vec3 target(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
float radius = 0.8f;
float angle = glm::half_pi<float>();


static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Update camera position and orientation based on user input
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        radius -= 0.3f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        radius += 0.3f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        angle -= 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        angle += 0.1f;
    }
}


int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Create a window and make its context current
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "CPU Bunny", NULL, NULL);
    GPU bunny(WIDTH, HEIGHT);

    // Set key callback
    glfwSetKeyCallback(window, key_callback);

    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        glfwTerminate(); //TODO: delete
        return -1;
    }

    // Set the clear color to black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    int x=0;
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        glm::vec3 cam = glm::vec3(radius * cos(angle), 0.0f, radius * sin(angle));
        glm::mat4 viewMatrix = glm::lookAt(cam, target, cameraUp);
        float *pixels = bunny.render(viewMatrix);

        // Draw the color buffer
        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_FLOAT, pixels);

        // Swap the front and back buffers
        glfwSwapBuffers(window);

        // Poll for events
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
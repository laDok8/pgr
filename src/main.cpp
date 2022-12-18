// Include the necessary OpenGL headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

// Include the standard C++ headers
#include <cstring>
#include <vector>

// Include the project headers
#include "model.h"
#include "GPU.h"


const int WIDTH = 640;
const int HEIGHT = 480;
glm::vec3 position(0.0f, 0.0f, 4.f);
glm::vec3 target(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);


static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {

    glm::vec3 cameraFront = -position;
    float cameraSpeed = 0.1f;


    // Update camera position and orientation based on user input
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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
        glfwTerminate();
        return -1;
    }

    // Set the clear color to black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 viewMatrix = glm::lookAt(position, target, cameraUp);
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
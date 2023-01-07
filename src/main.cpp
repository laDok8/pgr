#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <string>
#include <glm/ext/matrix_transform.hpp>
#include "model.h"
#include "GPU.h"

const int WIDTH = 1920;
const int HEIGHT = 1080;
//glm::vec3 position(0.0f, 0.0f, 4.f);
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
    if (!glfwInit())
        return -1;

    // Load the bunny model only once
    //TODO: change path if needed
    std::string objPath = "model/bunny.obj";
    std::string texturePath = "model/bunny.png";
    model m(objPath.c_str(), texturePath.c_str());

    // Create a window and make its context current
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "CPU Bunny", nullptr, nullptr);
    GPU bunny(WIDTH, HEIGHT, m);

    // Set key callback
    glfwSetKeyCallback(window, key_callback);

    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Set the clear color to black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        glm::vec3 cam = glm::vec3(radius * std::cos(angle), 0.0f, radius * std::sin(angle));
        glm::mat4 viewMatrix = glm::lookAt(cam, target, cameraUp);
        float *pixels = bunny.render(viewMatrix, cam);

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
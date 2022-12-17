// Include the necessary OpenGL headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


// Include the standard C++ headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <cstring>
#include <array>
#include <vector>
#include "model.h"
#include "GPU.h"


const int WIDTH = 640;
const int HEIGHT = 480;
glm::vec3 position(0.0f, 0.0f, 1.f);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
        position.x += 0.2f;
    else if(key == GLFW_KEY_S && action == GLFW_PRESS)
        position.x -= 0.2f;
    else if(key == GLFW_KEY_A && action == GLFW_PRESS)
        position.y += 0.2f;
    else if(key == GLFW_KEY_D && action == GLFW_PRESS)
        position.y -= 0.2f;
    else if(key == GLFW_KEY_Q && action == GLFW_PRESS)
        position.z += 0.2f;
    else if(key == GLFW_KEY_E && action == GLFW_PRESS)
        position.z -= 0.2f;
}






int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Create a window and make its context current
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "CPU Bunny", NULL, NULL);
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


        glm::mat4 viewMatrix = glm::lookAt(
                position, // camera position
                glm::vec3(0.0f, 0.0f, 0.0f), // target position
                glm::vec3(0.0f, 1.0f, 0.0f)  // up vector
        );

        viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
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
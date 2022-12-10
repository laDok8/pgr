// Include the necessary OpenGL headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Include the standard C++ headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

const int WIDTH = 640;
const int HEIGHT = 480;
float* g_color_buffer;

// Function to set the pixel at the given coordinates to the given color
void setPixel(int x, int y, float r, float g, float b) {
    // Calculate the index into the color buffer
    int index = (x + y * WIDTH) * 3;

    // Set the color of the pixel
    g_color_buffer[index + 0] = r;
    g_color_buffer[index + 1] = g;
    g_color_buffer[index + 2] = b;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Create a window and make its context current
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Rendering Pipeline", NULL, NULL);
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

    // Create the color buffer
    g_color_buffer = new float[WIDTH * HEIGHT * 3];

    // Set the clear color to black
    glClearColor(0.0f, 0.0f, 0.5f, 0.0f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Loop over all the pixels in the window
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                // Set the color of the pixel
                if ( x > y)
                    setPixel(x, y, 1.0f, 1.0f, 0.0f);
            }
        }

        // Draw the color buffer
        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_FLOAT, g_color_buffer);

        // Swap the front and back buffers
        glfwSwapBuffers(window);

        // Poll for events
        glfwPollEvents();
    }

    // Clean up
    delete[] g_color_buffer;
    glfwTerminate();
    return 0;
}
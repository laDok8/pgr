//
// Created by lada on 12/16/22.
//

#include <cstring>
#include <vector>
#include <array>
#include <glm/ext/matrix_projection.hpp>
#include "GPU.h"
#include "model.h"

GPU::GPU(const int WIDTH, const int HEIGHT) {
    this->WIDTH = WIDTH;
    this->HEIGHT = HEIGHT;
    color_buffer = new float[WIDTH * HEIGHT * 3];
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);;
    viewport = glm::vec4(0, 0, WIDTH, HEIGHT);
}

GPU::~GPU() {
    delete[] color_buffer;
}

// Function to set the pixel at the given coordinates to the given color
void GPU::setPixel(int x, int y, float r, float g, float b) {
    // Calculate the index into the color buffer
    int index = (x + y * WIDTH) * 3;

    // Set the color of the pixel
    color_buffer[index + 0] = r;
    color_buffer[index + 1] = g;
    color_buffer[index + 2] = b;
}

glm::vec3 GPU::barycentric(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 p)
{
    glm::vec3 bary;
    bary.x = ((v2.y - v3.y) * (p.x - v3.x) + (v3.x - v2.x) * (p.y - v3.y)) /
             ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y));
    bary.y = ((v3.y - v1.y) * (p.x - v3.x) + (v1.x - v3.x) * (p.y - v3.y)) /
             ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y));
    bary.z = 1.0f - bary.x - bary.y;
    //do the same thing using glm cross product
    return bary;
}

void GPU::rasterizeTriangle(const std::vector<std::array<glm::vec3, 3>>& triangles)
{
    std::vector<std::array<glm::vec3, 3>> transformedTriangles;
    for (const auto &triangle: triangles) {
        std::array<glm::vec3, 3> transformedTriangle;
        for (int i = 0; i < 3; i++) {
            glm::vec3 t1 = glm::project(triangle[i], viewMatrix, projectionMatrix, viewport);
            transformedTriangle[i] = t1;
        }
        transformedTriangles.push_back(transformedTriangle);
    }


    for(const auto &triangle: transformedTriangles){
        int minX = WIDTH - 1;
        int maxX = 0;
        int minY = HEIGHT - 1;
        int maxY = 0;
        for (int i = 0; i < 3; i++) {
            minX = std::min(minX, static_cast<int>(triangle[i].x));
            maxX = std::max(maxX, static_cast<int>(triangle[i].x));
            minY = std::min(minY, static_cast<int>(triangle[i].y));
            maxY = std::max(maxY, static_cast<int>(triangle[i].y));
        }
        // Iterate over bounding box and rasterize triangle
        for (int y = minY; y <= maxY; y++) {
            for (int x = minX; x <= maxX; x++) {
                glm::vec3 bary = barycentric(triangle[0], triangle[1], triangle[2], glm::vec3(x, y, 0.0f));
                if (bary.x < 0 || bary.y < 0 || bary.z < 0) continue;

                // Set pixel color using barycentric coordinates
                setPixel(x, y, 1.f,1.f,1.f);
            }
        }
    }

}

float * GPU::render(glm::mat4 viewMatrix) {
    this->viewMatrix = viewMatrix;
    memset(reinterpret_cast<wchar_t *>(color_buffer), 0, WIDTH * HEIGHT * 3 * sizeof(float));

    glm::vec3 v1 = glm::vec3(squareVertices[0].position[0], squareVertices[0].position[1], squareVertices[0].position[2]);
    glm::vec3 v2 = glm::vec3(squareVertices[1].position[0], squareVertices[1].position[1], squareVertices[1].position[2]);
    glm::vec3 v3 = glm::vec3(squareVertices[2].position[0], squareVertices[2].position[1], squareVertices[2].position[2]);
    std::vector<std::array<glm::vec3, 3>> triangles;
    triangles.push_back({v1, v2, v3});
    rasterizeTriangle(triangles);

    return color_buffer;

}
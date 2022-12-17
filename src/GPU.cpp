#include <cstring>
#include <vector>
#include <array>
#include <glm/ext/matrix_projection.hpp>
#include "model.h"
#include "GPU.h"


using namespace std;

GPU::GPU(const int WIDTH, const int HEIGHT) {
    this->WIDTH = WIDTH;
    this->HEIGHT = HEIGHT;
    color_buffer = new float[WIDTH * HEIGHT * 3];
    projectionMatrix = glm::perspective(glm::radians(45.0f), (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);;
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

glm::vec3 GPU::barycentric(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 p) {
    glm::vec3 bary;
    bary.x = ((v2.y - v3.y) * (p.x - v3.x) + (v3.x - v2.x) * (p.y - v3.y)) /
             ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y));
    bary.y = ((v3.y - v1.y) * (p.x - v3.x) + (v1.x - v3.x) * (p.y - v3.y)) /
             ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y));
    bary.z = 1.0f - bary.x - bary.y;
    return bary;
}

void GPU::rasterizeTriangle(const vector<primitive> primitives) {
    vector<array<glm::vec3, 3>> transformedTriangles;
    for (const auto &triangle: primitives) {
        array<glm::vec3, 3> transformedTriangle;
        for (int i = 0; i < 3; i++) {
            glm::vec3 t1 = glm::project(triangle[i].position, viewMatrix, projectionMatrix, viewport);
            transformedTriangle[i] = t1;
        }
        transformedTriangles.push_back(transformedTriangle);
    }


    for (const auto &triangle: transformedTriangles) {
        int minX = WIDTH - 1;
        int maxX = 0;
        int minY = HEIGHT - 1;
        int maxY = 0;
        for (int i = 0; i < 3; i++) {
            minX = min(minX, static_cast<int>(triangle[i].x));
            maxX = max(maxX, static_cast<int>(triangle[i].x));
            minY = min(minY, static_cast<int>(triangle[i].y));
            maxY = max(maxY, static_cast<int>(triangle[i].y));
        }
        // Iterate over bounding box and rasterize triangle
        for (int y = minY; y <= maxY; y++) {
            for (int x = minX; x <= maxX; x++) {
                glm::vec3 bary = barycentric(triangle[0], triangle[1], triangle[2], glm::vec3(x, y, 0.0f));
                if (bary.x < 0 || bary.y < 0 || bary.z < 0) continue;

                // Set pixel color using barycentric coordinates
                setPixel(x, y, 1.f, 1.f, 1.f);
            }
        }
    }

}


vector<primitive> GPU::getPrimitives() {
    vector<primitive> primitives;


    for (auto &indice: bunnyIndices) {
        primitive triangle = {bunnyVertices[indice[0]], bunnyVertices[indice[1]], bunnyVertices[indice[2]]};
        //TODO: clip a camera normal pohyb
        for (int i = 0; i < 3; i++) {
            triangle[i].position = triangle[i].position;
        }
        primitives.push_back(triangle);
    }


    return primitives;
}

float *GPU::render(glm::mat4 viewMatrix) {
    this->viewMatrix = viewMatrix;
    //clear buffer
    memset(reinterpret_cast<wchar_t *>(color_buffer), 0, WIDTH * HEIGHT * 3 * sizeof(float));


    //get primitives
    vector<primitive> triangles = getPrimitives();

    //rasterize
    rasterizeTriangle(triangles);

    return color_buffer;

}


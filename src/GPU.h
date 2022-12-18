#ifndef PGRPROJECT_GPU_H
#define PGRPROJECT_GPU_H

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "model.h"

class GPU {
public:
    GPU(const int WIDTH, const int HEIGHT, const model &m);

    ~GPU();

    float *render(glm::mat4 viewMatrix, glm::vec3 camera);

private:

    float *color_buffer, *depth_buffer;
    glm::mat4 viewMatrix;
    glm::vec3 camera;
    glm::mat4 projectionMatrix;
    int WIDTH, HEIGHT;
    model m;

    void setPixel(int x, int y, float r, float g, float b);

    glm::vec3 barycentric(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 p);

    std::vector<primitive> getPrimitives();

    void rasterizeTriangle(std::vector<primitive> primitives);

    oVertex VS(const iVertex &vertex);

    std::vector<primitive> clip(primitive p);

    glm::vec3 FS(iFrag pos);
};


#endif //PGRPROJECT_GPU_H

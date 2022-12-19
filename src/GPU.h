#ifndef PGRPROJECT_GPU_H
#define PGRPROJECT_GPU_H

#include <glm/glm.hpp>
#include "model.h"

class GPU {
public:
    GPU(int WIDTH, int HEIGHT, const model &m);

    ~GPU();

    float *render(glm::mat4 viewMatrix, glm::vec3 camera);

private:

    float *color_buffer, *depth_buffer;
    glm::mat4 viewMatrix{}, projectionMatrix{};
    glm::vec3 camera{};
    int WIDTH, HEIGHT;
    model m;

    static glm::vec3 barycentric(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 p);

    std::vector<primitive> getPrimitives();

    void rasterizeTriangle(const std::vector<primitive> &primitives);

    oVertex VS(const iVertex &vertex);

    static std::vector<primitive> clip(primitive p);

    glm::vec3 FS(iFrag pos);

    void setPixel(int x, int y, glm::vec3 color);
};


#endif //PGRPROJECT_GPU_H

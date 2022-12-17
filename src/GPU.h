#ifndef PGRPROJECT_GPU_H
#define PGRPROJECT_GPU_H

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "model.h"
//#include <glm/ext/matrix_clip_space.hpp>

class GPU {
public:
    GPU(const int WIDTH, const int HEIGHT);

    ~GPU();

    float *render(glm::mat4 viewMatrix);

private:


    float *color_buffer;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::vec4 viewport;
    int WIDTH, HEIGHT;

    void setPixel(int x, int y, float r, float g, float b);

    glm::vec3 barycentric(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 p);

    std::vector<primitive> getPrimitives();

    void rasterizeTriangle(const std::vector<primitive> primitives);

    std::vector<oPrimitive> clip(oPrimitive p);
};


#endif //PGRPROJECT_GPU_H

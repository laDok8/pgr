#ifndef PGRPROJECT_MODEL_H
#define PGRPROJECT_MODEL_H

#include <glm/glm.hpp>
#include <array>
#include <vector>

struct iVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 uv;    // 3rd coordinate for simpler processing
};
struct oVertex {
    glm::vec4 position;
    std::vector<glm::vec3> attrib;    // [pos, norm, uv]
};

struct iFrag {
    glm::vec4 position;
    std::vector<glm::vec3> attrib;    // [pos, norm, uv]
};

using VertexIndex = uint32_t;

typedef std::array<oVertex, 3> primitive;


class model {
public:
    model(const char *objPath, const char *texturePath);

    model();

    ~model();

    std::vector<struct iVertex> getPrimitives();

    std::vector<std::vector<VertexIndex>> getIndices();

    unsigned char *getTextureData();

    glm::vec2 getTextureSize() const;

    int getTextureChannels() const;


private:
    std::vector<struct iVertex> objDataVertex;
    std::vector<std::vector<VertexIndex>> objDataIndice;
    int textureWidth{}, textureHeight{}, textureChannels{};
    unsigned char *textureData{};
};


#endif //PGRPROJECT_MODEL_H
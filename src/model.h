#ifndef PGRPROJECT_MODEL_H
#define PGRPROJECT_MODEL_H

// functions to get vertices and number of given vertices in array
#include <cstdint>
#include <glm/glm.hpp>
#include <array>
#include <vector>
#include "model.h"


/**
 * @brief This structure represents vertex that contains only position and
 * normal.
 */
struct BunnyVertex {
    glm::vec3 position;  ///< position of vertex
    glm::vec3 normal;    ///< normal of vertex
    glm::vec3 uv;    /// UV 3rd codrinate for simpler procesing
};
struct oVertex {
    glm::vec4 position;  ///< position of vertex
    std::vector<glm::vec3> attrib;    // 0- pos 1- norm 2-uv
};

struct iFrag {
    glm::vec4 position;  ///< position of vertex
    std::vector<glm::vec3> attrib;    // 0- pos 1- norm 2-uv
};

/// This variable contains vertices of Standford bunny.
extern struct BunnyVertex const bunnyVertices[1048];

using VertexIndex = uint32_t;///< type of index

/// This variable contains Standford bunny indices.
extern const VertexIndex bunnyIndices[2092][3];

//typedef std::array<BunnyVertex, 3> primitive;
typedef std::array<oVertex, 3> primitive;
extern struct BunnyVertex const squareVertices[8];
extern const VertexIndex squareIndices[4][3];



class model {
    public:
    model();
    ~model();
    void prepareModel();
    std::vector<struct BunnyVertex> getPrimitives();
    std::vector<std::vector<VertexIndex>> getIndices();
    private:
    std::vector<struct BunnyVertex> objDataVertex;
    std::vector<std::vector<VertexIndex>> objDataIndice;
};


#endif //PGRPROJECT_MODEL_H

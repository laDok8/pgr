#ifndef PGRPROJECT_MODEL_H
#define PGRPROJECT_MODEL_H

// functions to get vertices and number of given vertices in array
#include <cstdint>
#include <glm/glm.hpp>
#include <array>
#include "model.h"


/**
 * @brief This structure represents vertex that contains only position and
 * normal.
 */
struct BunnyVertex {
    glm::vec3 position;  ///< position of vertex
    glm::vec3 normal;    ///< normal of vertex
};
struct oVertex {
    glm::vec4 position;  ///< position of vertex
};

/// This variable contains vertices of Standford bunny.
extern struct BunnyVertex const bunnyVertices[1048];

using VertexIndex = uint32_t;///< type of index

/// This variable contains Standford bunny indices.
extern const VertexIndex bunnyIndices[2092][3];

typedef std::array<BunnyVertex, 3> primitive;
typedef std::array<oVertex, 3> oPrimitive;
extern struct BunnyVertex const squareVertices[4];
extern const VertexIndex squareIndices[2][3];


#endif //PGRPROJECT_MODEL_H

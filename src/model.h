//
// Created by lada on 12/16/22.
//

#ifndef PGRPROJECT_MODEL_H
#define PGRPROJECT_MODEL_H

// functions to get vertices and number of given vertices in array
#include <cstdint>
#include <glm/vec3.hpp>


/**
 * @brief This structure represents vertex that contains only position and
 * normal.
 */
struct BunnyVertex {
    float position[3];  ///< position of vertex
    float normal[3];    ///< normal of vertex
};

/// This variable contains vertices of Standford bunny.
extern struct BunnyVertex const bunnyVertices[1048];

using VertexIndex = uint32_t;///< type of index
using VertexIndex2 = uint32_t;

/// This variable contains Standford bunny indices.
extern const VertexIndex bunnyIndices[2092][3];


struct SquareVertex {
    float position[3];  ///< position of vertex
    //float normal[3];    ///< normal of vertex
};
extern struct SquareVertex const squareVertices[4];
extern const VertexIndex2 squareIndices[2][3];



#endif //PGRPROJECT_MODEL_H

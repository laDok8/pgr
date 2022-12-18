#include "model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"


model::model(const char *objPath, const char *texturePath) {
    // Load the OBJ file using Assimp
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(objPath, aiProcess_Triangulate);
    //print current working directory

    // Check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    }

    // Get mesh data
    const aiMesh *mesh = scene->mMeshes[0];

    // Store the vertices and indices in your own data structures
    std::vector<float> vertices;
    vertices.reserve(mesh->mNumVertices * 3); // 3 coordinates per vertex
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        iVertex vertex;
        vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
        vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
        vertex.uv = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y, 1.f};
        objDataVertex.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace &face = mesh->mFaces[i];
        objDataIndice.push_back({face.mIndices[0], face.mIndices[1], face.mIndices[2]});
    }

    textureData = stbi_load(texturePath, &textureWidth, &textureHeight, &textureChannels, 0);
    if (!textureData) {
        std::cerr << "Failed to load texture " << stbi_failure_reason() << std::endl;
    }
}

std::vector<struct iVertex> model::getPrimitives() {
    return objDataVertex;
}

std::vector<std::vector<VertexIndex>> model::getIndices() {
    return objDataIndice;
}

model::~model() {

}

unsigned char *model::getTextureData() {
    return textureData;
}

glm::vec2 model::getTextureSize() {
    return glm::vec2(textureWidth, textureHeight);
}

int model::getTextureChannels() {
    return textureChannels;
}

model::model() {
    // for some reason called in GPU constructor;
}

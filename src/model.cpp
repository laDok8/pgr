#include "model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"


model::model(const char *objPath, const char *texturePath) {
    // load obj file using assimp
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(objPath, aiProcess_Triangulate);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "failure to load model " << importer.GetErrorString() << std::endl;
    }

    // whole model ( file should have only one mesh )
    const aiMesh *mesh = scene->mMeshes[0];

    // load vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        iVertex vertex{};
        vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
        vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
        vertex.uv = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y, 1.f};
        objDataVertex.push_back(vertex);
    }

    // load indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace &face = mesh->mFaces[i];
        objDataIndice.push_back({face.mIndices[0], face.mIndices[1], face.mIndices[2]});
    }

    // load texture using stb_image
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

model::~model() = default;

unsigned char *model::getTextureData() {
    return textureData;
}

glm::vec2 model::getTextureSize() const {
    return {textureWidth, textureHeight};
}

int model::getTextureChannels() const {
    return textureChannels;
}

model::model() {
    // for some reason called in GPU constructor;
}

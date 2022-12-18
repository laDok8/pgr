#include <cstring>
#include <vector>
#include <array>
#include <glm/ext/matrix_projection.hpp>
#include "GPU.h"
#include "model.h"
#include <GLFW/glfw3.h>


using namespace std;

GPU::GPU(const int WIDTH, const int HEIGHT, const model &m) {
    this->WIDTH = WIDTH;
    this->HEIGHT = HEIGHT;
    color_buffer = new float[WIDTH * HEIGHT * 3];
    depth_buffer = new float[WIDTH * HEIGHT * 3];
    projectionMatrix = glm::perspective(45.0f, (float) WIDTH / (float) HEIGHT, 0.1f, 100.0f);
    this->m = m;
}

GPU::~GPU() {
    delete[] color_buffer;
    delete[] depth_buffer;
}

void GPU::setPixel(int x, int y, float r, float g, float b) {
    int index = (x + y * WIDTH) * 3;

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

void GPU::rasterizeTriangle(vector<primitive> primitives) {
    for (const auto &triangle: primitives) {
        int minX = WIDTH - 1;
        int maxX = 0;
        int minY = HEIGHT - 1;
        int maxY = 0;
        for (int i = 0; i < 3; i++) {
            minX = min(minX, static_cast<int>(triangle[i].position.x));
            maxX = max(maxX, static_cast<int>(triangle[i].position.x));
            minY = min(minY, static_cast<int>(triangle[i].position.y));
            maxY = max(maxY, static_cast<int>(triangle[i].position.y));
        }
        //TODO: popsat algo
        // Iterate over bounding box and rasterize triangle
        for (int y = minY; y <= maxY; y++) {
            for (int x = minX; x <= maxX; x++) {
                if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) continue;

                glm::vec3 bary = barycentric(triangle[0].position, triangle[1].position, triangle[2].position,
                                             glm::vec3(x, y, 0.0f));
                //inside triangle
                if (bary.x < 0 || bary.y < 0 || bary.z < 0) continue;

                iFrag myFragment;
                myFragment.position = {x + 0.5f, y + 0.5f, 1, 1};


                float z = bary.x * triangle[0].position.z + bary.y * triangle[1].position.z +
                          bary.z * triangle[2].position.z;
                myFragment.position.z = z;

                //depth test
                if (depth_buffer[x + y * WIDTH] <= z) continue;


                //interpolate attributes
                for (int i = 0; i < triangle[0].attrib.size(); i++) {
                    myFragment.attrib.push_back({bary.x * triangle[0].attrib[i] + bary.y * triangle[1].attrib[i] +
                                                 bary.z * triangle[2].attrib[i]});
                }

                glm::vec3 color = FS(myFragment);

                // Set pixel color using barycentric coordinates
                setPixel(x, y, color.x, color.y, color.z);
                depth_buffer[x + y * WIDTH] = z;
            }
        }
    }
}

vector<primitive> GPU::clip(primitive p) {

    vector<primitive> clippedPrimitives;
    std::vector<oVertex> overtex({p[0], p[1], p[2]});
    struct oVertex help;
    //TODO: rework

    //CLIPING PART
    float t;
    //calculating new points + interpolation
    for (int i = 0; i < 3; i++) {
        float divvect;
        //choosing vector
        if (i == 2)
            divvect = overtex[0].position[3] - overtex[2].position[3] + overtex[0].position[2] - overtex[2].position[2];
        else
            divvect = overtex[i + 1].position[3] - overtex[i].position[3] + overtex[i + 1].position[2] -
                      overtex[i].position[2];

        t = (-overtex[i].position[3] - overtex[i].position[2]) / divvect;
        //toCull
        if (t > 0 && t < 1) {

            if (i == 2)
                help.position = overtex[2].position + t * (overtex[0].position - overtex[2].position);
            else {
                help.position = overtex[i].position + t * (overtex[i + 1].position - overtex[i].position);
            }
            //interpolate attributes
            for (int z = 0; z < overtex[0].attrib.size(); z++) {
                help.attrib.push_back(overtex[2].attrib[z] + t * (overtex[2].attrib[z] - overtex[0].attrib[z]));
            }
            overtex.push_back(help);
        }
    }

    bool delFlag = false;
    //delete old vertices
    for (int j = 2; j >= 0; j--) {
        if (-overtex[j].position[3] > overtex[j].position[2]) {
            overtex.erase(overtex.begin() + j);
            delFlag = true;

        }
    }

    //empty
    const glm::vec4 bad{0, 0, 0, 0};
    if (overtex[0].position == bad) {
        return clippedPrimitives;
    }

    //asembly
    if (overtex.size() == 3) {
        if (!delFlag)
            clippedPrimitives.push_back({overtex[0], overtex[1], overtex[2]});
        else
            clippedPrimitives.push_back({overtex[2], overtex[1], overtex[0]});
    }
    if (overtex.size() == 4) {
        clippedPrimitives.push_back({overtex[0], overtex[1], overtex[2]});
        clippedPrimitives.push_back({overtex[0], overtex[2], overtex[3]});

    }
    return clippedPrimitives;
}


//returns assembled primitives in clip space
vector<primitive> GPU::getPrimitives() {
    vector<primitive> triangles;

    auto indices = m.getIndices();
    auto vertices = m.getPrimitives();
    for (auto &indice: indices) {
        primitive triangle = {VS(vertices[indice[0]]), VS(vertices[indice[1]]), VS(vertices[indice[2]])};
        triangles.push_back(triangle);
    }


    vector<primitive> clippedTriangles;
    //clipping
    for (auto &triangle: triangles) {
        //clip may result to 0-2 triangles
        vector<primitive> clipped = clip(triangle);
        for (auto &t: clipped) {
            clippedTriangles.push_back(t);
        }
    }


    vector<primitive> transformedTriangles;
    //transform to screen space
    for (const auto &triangle: clippedTriangles) {
        primitive transformedTriangle;
        for (int i = 0; i < 3; i++) {
            transformedTriangle[i].attrib = triangle[i].attrib;
            glm::vec3 t1;
            t1.x = (triangle[i].position.x / triangle[i].position.w + 1.0f) * WIDTH / 2.0f;
            t1.y = (triangle[i].position.y / triangle[i].position.w + 1.0f) * HEIGHT / 2.0f;
            t1.z = triangle[i].position.z / triangle[i].position.w;
            transformedTriangle[i].position = glm::vec4(t1, 1.0f);//W is not necessary
        }
        transformedTriangles.push_back(transformedTriangle);
    }

    return transformedTriangles;
}

float *GPU::render(glm::mat4 viewMatrix, glm::vec3 camera) {
    int width, height, numChannels;

    this->viewMatrix = viewMatrix;
    this->camera = camera;
    //clear buffer (easier than 0.f and 1.1f)
    memset(reinterpret_cast<wchar_t *>(color_buffer), WCHAR_MIN, WIDTH * HEIGHT * 3 * sizeof(float));
    memset(reinterpret_cast<wchar_t *>(depth_buffer), WCHAR_MAX, WIDTH * HEIGHT * 3 * sizeof(float));


    //get primitives
    vector<primitive> triangles = getPrimitives();

    //rasterize
    rasterizeTriangle(triangles);

    return color_buffer;

}

glm::vec3 GPU::FS(iFrag frag) {

    glm::vec3 outFragment;

    glm::vec3 pos = frag.attrib[0];
    glm::vec3 norm = frag.attrib[1];
    glm::vec2 uv = frag.attrib[2];
    glm::vec3 light = glm::vec3(10.f, 10.f, 10.f); // hardcoded light position
    //glm::vec3 camera(0.0f, 0.0f, 4.f);//TODO BEWARE

    //texture
    unsigned char *data = m.getTextureData();

    int x = uv.x * m.getTextureSize().x;
    int y = uv.y * m.getTextureSize().y;
    int idx = (y * m.getTextureSize().x + x) * m.getTextureChannels();
    glm::vec3 texColor = glm::vec3(data[idx], data[idx + 1], data[idx + 2]) / 255.f;


    //PhongLighting
    glm::vec3 lightVectNorm = glm::normalize(light - pos);
    float diffuse = glm::dot(glm::normalize(norm), lightVectNorm);
    if (diffuse <= 0.f) {
        return glm::vec3(0.f);
    }

    float specular = glm::pow(
            glm::clamp(glm::dot(lightVectNorm, -glm::reflect(glm::normalize(camera - pos), norm)), 0.f, 1.f), 40.f);
    glm::vec3 final = glm::clamp(diffuse * texColor + specular, 0.f, 1.f);
    return final;
}

oVertex GPU::VS(const iVertex &vertex) {
    glm::mat4 mvp = projectionMatrix * viewMatrix * glm::mat4(1.0f);
    oVertex outVertex;
    outVertex.position = mvp * glm::vec4(vertex.position, 1.0f);
    outVertex.attrib.push_back(vertex.position);
    outVertex.attrib.push_back(vertex.normal);
    outVertex.attrib.push_back(vertex.uv);

    return outVertex;
}
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

void write()
{
    std::vector<Vertex> vertices =
    {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        2, 3, 0
    };

    std::ofstream file("../Sandbox/example.mesh");

    size_t vsize = vertices.size() * sizeof(Vertex);
    std::string vname = "VERTICES";
    file.write(vname.c_str(), 8);
    file.write((const char*)&vsize, 8);
    for (auto& v : vertices)
    {
        file.write((const char*)&v.position, 12);
        file.write((const char*)&v.normal, 12);
        file.write((const char*)&v.uv, 8);
    }

    size_t isize = indices.size() * sizeof(unsigned int);
    std::string iname = "INDICES ";
    file.write(iname.c_str(), 8);
    file.write((const char*)&isize, 8);
    for (auto& i : indices)
    {
        file.write((const char*)&i, 4);
    }

    int width, height, channels;
    unsigned char* data = stbi_load("../Sandbox/tex.png", &width, &height, &channels, 0);
    std::string tname = "TEXTURE ";
    size_t tsize = width * height * channels;
    file.write(tname.c_str(), 8);
    file.write((const char*)&tsize, 8);
    file.write((const char*)&width, 4);
    file.write((const char*)&height, 4);
    file.write((const char*)&channels, 4);
    file.write((const char*)data, tsize);
    file.close();
    stbi_image_free(data);
}

void read()
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::ifstream file("../Sandbox/example.mesh");

    char vname[9];
    vname[8] = '\0';
    size_t vsize;
    file.read((char*)&vname, 8);
    file.read((char*)&vsize, 8);
    size_t vcounter = vsize;
    while (vcounter > 0)
    {
        Vertex v;
        file.read((char*)&v.position, 12);
        file.read((char*)&v.normal, 12);
        file.read((char*)&v.uv, 8);
        vertices.push_back(v);
        vcounter -= 32;
    }

    char iname[9];
    iname[8] = '\0';
    size_t isize;
    file.read((char*)&iname, 8);
    file.read((char*)&isize, 8);
    size_t icounter = isize;
    while (icounter > 0)
    {
        unsigned int i;
        file.read((char*)&i, 4);
        indices.push_back(i);
        icounter -= 4;
    }

    printf("%s, %lld\n", vname, vsize);
    for (auto& v : vertices)
    {
        printf("(%.1f, %.1f, %.1f) (%.1f, %.1f, %.1f) (%.1f, %.1f)\n",
            v.position.x,
            v.position.y,
            v.position.z,
            v.normal.x,
            v.normal.y,
            v.normal.z,
            v.uv.x,
            v.uv.y
        );
    }

    printf("%s, %lld\n", iname, isize);
    for (auto& i : indices)
    {
        printf("%d, ", i);
    }

    file.close();
}

int main()
{
    write();
    read();
}
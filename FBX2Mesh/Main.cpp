#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <fstream>
#include <string>
#include <vector>

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

void ProcessMesh(aiMesh *a_mesh, const aiScene *scene)
{
}

int main()
{
    const char *path = "../FBX2Mesh/Tiger.fbx";

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path,
                                             aiProcess_CalcTangentSpace |
                                                 aiProcess_Triangulate |
                                                 aiProcess_FlipUVs |
                                                 aiProcess_JoinIdenticalVertices |
                                                 aiProcess_SortByPType);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    aiMesh *mesh = scene->mMeshes[0];
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex v;
        v.position.x = mesh->mVertices[i].x;
        v.position.y = mesh->mVertices[i].y;
        v.position.z = mesh->mVertices[i].z;
        v.normal.x = mesh->mNormals[i].x;
        v.normal.y = mesh->mNormals[i].y;
        v.normal.z = mesh->mNormals[i].z;
        v.uv.x = mesh->mTextureCoords[0][i].x;
        v.uv.y = mesh->mTextureCoords[0][i].y;
        vertices.push_back(v);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial *material = scene->mMaterials[0];
    aiString texture_file;
    material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texture_file);
    auto texture = scene->GetEmbeddedTexture(texture_file.C_Str());

    printf("%s\n", texture->mFilename.C_Str());
    printf("%d, %d\n", texture->mWidth, texture->mHeight);
    aiTexel *texel = texture->pcData;

    std::ofstream file("../FBX2Mesh/teste.mesh", std::ios::binary);

    size_t vsize = vertices.size() * sizeof(Vertex);
    std::string vname = "VERTICES";
    file.write(vname.c_str(), 8);
    file.write((const char *)&vsize, 8);
    for (auto &v : vertices)
    {
        file.write((const char *)&v.position, 12);
        file.write((const char *)&v.normal, 12);
        file.write((const char *)&v.uv, 8);
    }

    size_t isize = indices.size() * sizeof(unsigned int);
    std::string iname = "INDICES ";
    file.write(iname.c_str(), 8);
    file.write((const char *)&isize, 8);
    for (auto &i : indices)
    {
        file.write((const char *)&i, 4);
    }

    int width, height, channels;
    unsigned char *data = stbi_load_from_memory((const stbi_uc *)texel, texture->mWidth, &width, &height, &channels, 0);

    std::string tname = "TEXTURE ";
    size_t tsize = width * height * channels;
    printf("SIZE: %lld\n\n\n", tsize);
    file.write(tname.c_str(), 8);
    file.write((const char *)&tsize, 8);
    file.write((const char *)&width, 4);
    file.write((const char *)&height, 4);
    file.write((const char *)&channels, 4);
    file.write((const char *)data, tsize);
    file.close();

    printf("%s, %lld\n", vname.c_str(), vsize);
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

    printf("%s, %lld\n", iname.c_str(), isize);
    for (auto& i : indices)
    {
        printf("%d, ", i);
    }
}
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/quaternion.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <iostream>
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
    glm::vec3 position = glm::vec3(0);
    glm::vec3 normal = glm::vec3(0);
    glm::vec2 uv = glm::vec2(0);
    glm::ivec4 bone = glm::ivec4(0);
    glm::vec4 weight = glm::vec4(0);
};

struct Bone
{
    std::string name;
    glm::mat4 offset;
    std::vector<Bone> children;
};

class Animation
{
public:
    struct Keyframe
    {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        float time;
    };
    struct BoneKey
    {
        std::string name;
        std::vector<Keyframe> keyframes;
    };

public:
    std::string name;
    std::vector<BoneKey> bonekeys;
    float length;
};

void ReadBones(aiNode *node, Bone &bone, std::vector<Bone> &bones);
void PrintBone(Bone bone, std::ofstream &file);

int main()
{
    const char *path = "../FBX2SMesh/untitled.fbx";

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path,
                                             aiProcess_CalcTangentSpace |
                                                 aiProcess_Triangulate |
                                                 aiProcess_FlipUVs |
                                                 aiProcess_JoinIdenticalVertices |
                                                 aiProcess_SortByPType);

    auto mt = scene->mRootNode->mTransformation;
    glm::mat4 globalInverseTransform = glm::inverse(glm::mat4(
        mt.a1, mt.b1, mt.c1, mt.d1,
        mt.a2, mt.b2, mt.c2, mt.d2,
        mt.a3, mt.b3, mt.c3, mt.d3,
        mt.a4, mt.b4, mt.c4, mt.d4));

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Bone> bones;
    std::vector<Animation> animations;

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

    if (mesh->HasBones())
    {
        for (unsigned int i = 0; i < mesh->mNumBones; i++)
        {
            unsigned int boneIndex = 0;
            Bone bone;
            bone.name = mesh->mBones[i]->mName.data;
            auto offsetMatrix = mesh->mBones[i]->mOffsetMatrix;
            bone.offset = glm::mat4(
                offsetMatrix.a1, offsetMatrix.b1, offsetMatrix.c1, offsetMatrix.d1,
                offsetMatrix.a2, offsetMatrix.b2, offsetMatrix.c2, offsetMatrix.d2,
                offsetMatrix.a3, offsetMatrix.b3, offsetMatrix.c3, offsetMatrix.d3,
                offsetMatrix.a4, offsetMatrix.b4, offsetMatrix.c4, offsetMatrix.d4);
            bones.push_back(bone);

            for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
            {
                unsigned int vertexId = mesh->mBones[i]->mWeights[j].mVertexId;
                float weight = mesh->mBones[i]->mWeights[j].mWeight;
                if (vertices[vertexId].weight.x == 0.0f)
                {
                    vertices[vertexId].weight.x = weight;
                    vertices[vertexId].bone.x = i;
                }
                else if (vertices[vertexId].weight.y == 0.0f)
                {
                    vertices[vertexId].weight.y = weight;
                    vertices[vertexId].bone.y = i;
                }
                else if (vertices[vertexId].weight.z == 0.0f)
                {
                    vertices[vertexId].weight.z = weight;
                    vertices[vertexId].bone.z = i;
                }
                else if (vertices[vertexId].weight.w == 0.0f)
                {
                    vertices[vertexId].weight.w = weight;
                    vertices[vertexId].bone.w = i;
                }
            }
        }
    }

    aiMaterial *material = scene->mMaterials[0];
    aiString texture_file;
    material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texture_file);
    auto texture = scene->GetEmbeddedTexture(texture_file.C_Str());
    aiTexel *texel = texture->pcData;

    aiNode *armature;
    for (int i = 0; i < scene->mRootNode->mNumChildren; i++)
    {
        if (std::string(scene->mRootNode->mChildren[i]->mName.C_Str()) == "Armature")
            armature = scene->mRootNode->mChildren[i];
    }
    Bone root = bones[0];
    ReadBones(armature->mChildren[0], root, bones);

    for (int a = 0; a < scene->mNumAnimations; a++)
    {
        Animation animation;
        aiAnimation *anim = scene->mAnimations[a];
        animation.name = anim->mName.C_Str();
        for (int i = 0; i < anim->mNumChannels; i++)
        {
            aiNodeAnim *nodeAnim = anim->mChannels[i];
            Animation::BoneKey bonekey;
            bonekey.name = nodeAnim->mNodeName.C_Str();
            for (int j = 0; j < nodeAnim->mNumPositionKeys; j++)
            {
                Animation::Keyframe keyframe;
                aiVectorKey positionKey = nodeAnim->mPositionKeys[j];
                aiQuatKey rotationKey = nodeAnim->mRotationKeys[j];
                aiVectorKey scaleKey = nodeAnim->mScalingKeys[j];
                aiQuaternion quat = rotationKey.mValue;
                glm::vec3 rotation;
                rotation.x = atan2(2 * (quat.w * quat.x + quat.y * quat.z), 1 - 2 * (quat.x * quat.x + quat.y * quat.y));
                rotation.y = asin(2 * (quat.w * quat.y - quat.z * quat.x));
                rotation.z = atan2(2 * (quat.w * quat.z + quat.x * quat.y), 1 - 2 * (quat.y * quat.y + quat.z * quat.z));

                keyframe.position.x = positionKey.mValue.x;
                keyframe.position.y = positionKey.mValue.y;
                keyframe.position.z = positionKey.mValue.z;
                keyframe.rotation.x = glm::degrees(rotation.x);
                keyframe.rotation.y = glm::degrees(rotation.y);
                keyframe.rotation.z = glm::degrees(rotation.z);
                keyframe.scale.x = scaleKey.mValue.x;
                keyframe.scale.y = scaleKey.mValue.y;
                keyframe.scale.z = scaleKey.mValue.z;

                keyframe.time = positionKey.mTime;

                bonekey.keyframes.push_back(keyframe);
            }
            animation.length = nodeAnim->mPositionKeys[nodeAnim->mNumPositionKeys - 1].mTime;
            animation.bonekeys.push_back(bonekey);
        }
        animations.push_back(animation);
    }

    std::ofstream file("../FBX2SMesh/teste.smesh", std::ios::binary);

    size_t vsize = vertices.size() * sizeof(Vertex);
    std::string vname = "VERTICES";
    file.write(vname.c_str(), 8);
    file.write((const char *)&vsize, 8);
    for (auto &v : vertices)
    {
        file.write((const char *)&v.position, 12);
        file.write((const char *)&v.normal, 12);
        file.write((const char *)&v.uv, 8);
        file.write((const char *)&v.bone, 16);
        file.write((const char *)&v.weight, 16);
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
    file.write(tname.c_str(), 8);
    file.write((const char *)&tsize, 8);
    file.write((const char *)&width, 4);
    file.write((const char *)&height, 4);
    file.write((const char *)&channels, 4);
    file.write((const char *)data, tsize);

    stbi_image_free(data);

    std::string bname = "BONES   ";
    size_t bsize = bones.size() * (12 + 4 + 64);
    file.write(bname.c_str(), 8);
    file.write((const char *)&bsize, 8);

    PrintBone(root, file);

    printf("%s\n", animations[1].name.c_str());
    printf("%s\n", animations[1].bonekeys[0].name.c_str());
    for (auto &keyframe : animations[1].bonekeys[0].keyframes)
    {
        printf("(%.1f, %.1f, %.1f)(%.1f, %.1f, %.1f)(%.1f, %.1f, %.1f) - %.1f\n",
               keyframe.position.x, keyframe.position.y, keyframe.position.z,
               keyframe.rotation.x, keyframe.rotation.y, keyframe.rotation.z,
               keyframe.scale.x, keyframe.scale.y, keyframe.scale.z,
               keyframe.time);
    }

    file.close();
}

void PrintBone(Bone bone, std::ofstream &file)
{
    file.write(bone.name.c_str(), 12);
    size_t childrens = bone.children.size();
    file.write((const char *)&childrens, 4);
    file.write((const char *)&bone.offset, 64);
    for (auto &c : bone.children)
    {
        PrintBone(c, file);
    }
}

void ReadBones(aiNode *node, Bone &bone, std::vector<Bone> &bones)
{
    for (int i = 0; i < node->mNumChildren; i++)
    {
        for (Bone &b : bones)
        {
            if (b.name == node->mChildren[i]->mName.C_Str())
            {
                ReadBones(node->mChildren[i], b, bones);
                bone.children.push_back(b);
                break;
            }
        }
    }
}
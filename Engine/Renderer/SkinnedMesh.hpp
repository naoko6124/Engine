#pragma once
#include "pch.hpp"
#include "Entity.hpp"
#include "Bone.hpp"
#include "Animation.hpp"

namespace Engine
{
    class SkinnedMesh : public Entity
    {
    public:
        struct Vertex
        {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 uv;
            glm::ivec4 bone;
            glm::vec4 weight;

            Vertex(glm::vec3 p, glm::vec3 n, glm::vec2 u, glm::ivec4 b, glm::vec4 w)
            {
                position = p;
                normal = n;
                uv = u;
                bone = b;
                weight = w;
            }
        };
    public:
        SkinnedMesh()
        {
            glGenVertexArrays(1, &vertexArray);
        }
        ~SkinnedMesh()
        {
            glDeleteVertexArrays(1, &vertexArray);
        }
        void SetVertices(const std::vector<Vertex>& vertices)
        {
            glBindVertexArray(vertexArray);
            unsigned int vertexBuffer;
            glGenBuffers(1, &vertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
            glEnableVertexAttribArray(2);
            glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, bone));
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weight));
            glEnableVertexAttribArray(4);
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDeleteBuffers(1, &vertexBuffer);
        }
        void SetIndices(const std::vector<uint32_t>& indices)
        {
            size = indices.size();
            glBindVertexArray(vertexArray);
            unsigned int indexBuffer;
            glGenBuffers(1, &indexBuffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
            glBindVertexArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glDeleteBuffers(1, &indexBuffer);
        }
        void SetTexture(const char* path)
        {
            int width, height, channels;
            unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            if (channels == 4)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            if (channels == 3)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glBindTexture(GL_TEXTURE_2D, 0);
            stbi_image_free(data);
        }
        void ApplyAnimation(Animation animation, float time)
        {
            float cuttime = time - floor(time / animation.length) * animation.length;
            ApplyAnimationOnBone(root, animation, cuttime);
        }
        void CalculateBoneTransform()
        {
            CalculateBone(root, glm::mat4(1));
        }
        void Render()
        {
            glBindVertexArray(vertexArray);
            glBindTexture(GL_TEXTURE_2D, texture);
            glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindVertexArray(0);
        }
    public:
        Bone root;
    private:
        void CalculateBone(Bone& bone, glm::mat4 parentTransform)
        {
            bone.finalTransform = parentTransform *
            bone.offset.GetTransformationMatrix() *
            bone.transform.GetTransformationMatrix() *
            glm::inverse(bone.offset.GetTransformationMatrix());

            for (Bone& child : bone.children)
                CalculateBone(child, bone.finalTransform);
        }
        void ApplyAnimationOnBone(Bone& bone, Animation animation, float time)
        {
            auto bonekey = animation.GetBoneKey(bone.name);
            if (bonekey != nullptr)
            {
                int index = 0;
                Animation::Keyframe kf = bonekey->keyframes[index];
                while (index < bonekey->keyframes.size())
                {
                    if (time < kf.time)
                    {
                        bone.transform = Animation::Interpolate(kf, bonekey->keyframes[index - 1], time);
                        break;
                    }
                    index++;
                    kf = bonekey->keyframes[index];
                }
            }
            for (Bone& child : bone.children)
                ApplyAnimationOnBone(child, animation, time);
        }
    private:
        unsigned int vertexArray;
        unsigned int texture;
        unsigned int size;
    };
}
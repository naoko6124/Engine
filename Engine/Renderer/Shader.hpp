#pragma once
#include "pch.hpp"
#include "Bone.hpp"
#include "Core/Logger.hpp"

namespace Engine
{
    class Shader
    {
    public:
        void Init()
        {
            program = glCreateProgram();
        }
        void Terminate()
        {
            glDeleteProgram(program);
        }
        void LoadVS(std::string code)
        {
            unsigned int vertexShader;
            vertexShader = glCreateShader(GL_VERTEX_SHADER);
            const char* vertexCode = code.c_str();
            glShaderSource(vertexShader, 1, &vertexCode, 0);
            glCompileShader(vertexShader);

            glAttachShader(program, vertexShader);
            glLinkProgram(program);
            glDeleteShader(vertexShader);
        }
        void LoadFS(std::string code)
        {
            unsigned int fragmentShader;
            fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            const char* fragmentCode = code.c_str();
            glShaderSource(fragmentShader, 1, &fragmentCode, 0);
            glCompileShader(fragmentShader);

            glAttachShader(program, fragmentShader);
            glLinkProgram(program);
            glDeleteShader(fragmentShader);
        }
        void Use()
        {
            glUseProgram(program);
        }
        void UploadMat4(std::string location, glm::mat4 value)
        {
            int loc = glGetUniformLocation(program, location.c_str());
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
        }
        void UploadBones(Bone root)
        {
            boneIndex = 0;
            UploadBone(root);
        }
    private:
        void UploadBone(Bone bone)
        {
            UploadMat4("bones[" + std::to_string(boneIndex) + "]", bone.finalTransform);
            boneIndex++;
            for (Bone child : bone.children)
                UploadBone(child);
        }
    private:
        unsigned int program;
        int boneIndex;
    };
}
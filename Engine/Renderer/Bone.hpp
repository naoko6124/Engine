#pragma once
#include "pch.hpp"
#include "Entity.hpp"

namespace Engine
{
    class Bone : public Entity
    {
    public:
        std::string name;
        glm::mat4 finalTransform;
        std::vector<Bone> children;
        Transform offset;
    };
}
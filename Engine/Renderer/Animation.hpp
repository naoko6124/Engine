#pragma once
#include "pch.hpp"
#include "Transform.hpp"

namespace Engine
{
    class Animation
    {
    public:
        struct Keyframe
        {
            Transform transform;
            float time;
            Keyframe(Transform tr, float t) { transform = tr; time = t; }
        };
        struct BoneKey
        {
            std::string name;
            std::vector<Keyframe> keyframes;
            BoneKey(std::string n, std::vector<Keyframe> k) { name = n; keyframes = k; }
        };
    public:
        BoneKey* GetBoneKey(std::string n)
        {
            for (BoneKey& b : bonekeys)
                if (b.name == n)
                    return &b;
            return nullptr;
        }
        static Transform Interpolate(Keyframe t1, Keyframe t2, float time)
        {
            Transform result;
            float fulldistance = abs(t2.time - t1.time);
            float t1distance = abs(t1.time - time);
            float t2distance = abs(t2.time - time);
            result.rotation.z = t1.transform.rotation.z * (t2distance/fulldistance)
                              + t2.transform.rotation.z * (t1distance/fulldistance);
            return result;
        }
    public:
        std::vector<BoneKey> bonekeys;
        float length;
    };
}
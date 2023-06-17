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
            Keyframe() { }
            Keyframe(Transform tr, float t) { transform = tr; time = t; }
        };
        struct BoneKey
        {
            std::string name;
            std::vector<Keyframe> keyframes;
            BoneKey() { }
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
            float t2p = abs(t1.time - time)/fulldistance;
            float t1p = abs(t2.time - time)/fulldistance;
            // result.position.x = t1.transform.position.x * t1p
            //                   + t2.transform.position.x * t2p;
            // result.position.y = t1.transform.position.y * t1p
            //                   + t2.transform.position.y * t2p;
            // result.position.z = t1.transform.position.z * t1p
            //                   + t2.transform.position.z * t2p;
            result.rotation.x = t1.transform.rotation.x * t1p
                              + t2.transform.rotation.x * t2p;
            result.rotation.y = t1.transform.rotation.y * t1p
                              + t2.transform.rotation.y * t2p;
            result.rotation.z = t1.transform.rotation.z * t1p
                              + t2.transform.rotation.z * t2p;
            return result;
        }
    public:
        std::string name;
        std::vector<BoneKey> bonekeys;
        float length;
    };
}
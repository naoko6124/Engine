#pragma once
#include "pch.hpp"
#include "Renderer/Mesh.hpp"
#include "Renderer/SkinnedMesh.hpp"
#include "Renderer/Bone.hpp"

namespace Engine
{
    class Assets
    {
    public:
        static std::string LoadFile(const char* path)
        {
            std::ifstream file;
            file.open(path);
            std::stringstream text_stream;
            text_stream << file.rdbuf();
            file.close();
            std::string text = text_stream.str();
            return text;
        }
        static Mesh LoadMesh(const char* path)
        {
            Mesh m;
            std::ifstream file(path, std::ios::binary);

            std::vector<Mesh::Vertex> vertices;
            std::vector<unsigned int> indices;

            char vname[9];
            vname[8] = '\0';
            size_t vsize;
            file.read((char*)&vname, 8);
            file.read((char*)&vsize, 8);
            size_t vcounter = vsize;
            while (vcounter > 0)
            {
                Mesh::Vertex v;
                file.read((char*)&v.position, 12);
                file.read((char*)&v.normal, 12);
                file.read((char*)&v.uv, 8);
                vertices.push_back(v);
                vcounter -= 32;
            }

            m.SetVertices(vertices);

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

            m.SetIndices(indices);

            char tname[9];
            tname[8] = '\0';
            size_t tsize;
            file.read((char*)&tname, 8);
            file.read((char*)&tsize, 8);
            int width, height, channels;
            file.read((char*)&width, 4);
            file.read((char*)&height, 4);
            file.read((char*)&channels, 4);
            char* data = new char[tsize];
            file.read((char*)data, tsize);

            m.SetTexture(data, width, height, channels);

            delete[] data;

            file.close();
            return m;
        }
        static SkinnedMesh LoadSkinnedMesh(const char* path)
        {
            SkinnedMesh m;
            std::ifstream file(path, std::ios::binary);

            std::vector<SkinnedMesh::Vertex> vertices;
            std::vector<unsigned int> indices;

            char vname[9];
            vname[8] = '\0';
            size_t vsize;
            file.read((char*)&vname, 8);
            file.read((char*)&vsize, 8);
            size_t vcounter = vsize;
            while (vcounter > 0)
            {
                SkinnedMesh::Vertex v;
                file.read((char*)&v.position, 12);
                file.read((char*)&v.normal, 12);
                file.read((char*)&v.uv, 8);
                file.read((char*)&v.bone, 16);
                file.read((char*)&v.weight, 16);
                vertices.push_back(v);
                vcounter -= 64;
            }

            m.SetVertices(vertices);

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

            m.SetIndices(indices);

            char tname[9];
            tname[8] = '\0';
            size_t tsize;
            file.read((char*)&tname, 8);
            file.read((char*)&tsize, 8);
            int width, height, channels;
            file.read((char*)&width, 4);
            file.read((char*)&height, 4);
            file.read((char*)&channels, 4);
            char* data = new char[tsize];
            file.read((char*)data, tsize);

            m.SetTexture(data, width, height, channels);

            delete[] data;

            char bname[9];
            bname[8] = '\0';
            size_t bsize;
            file.read((char*)&bname, 8);
            file.read((char*)&bsize, 8);

            ReadBone(m.root, file, "");

            file.close();
            return m;
        }
        static void ReadBone(Bone& bone, std::ifstream& file, std::string level)
        {
            std::cout << file.tellg();

            char bname[13];
            bname[12] = '\0';
            int children;
            file.read((char*)&bname, 12);
            file.read((char*)&children, 4);
            bone.name = bname;
            file.read((char*)&bone.offset, 64);

            std::cout << glm::to_string(bone.offset) << "\n";
            level.append("-");

            if (children == 0)
                return;

            bone.children.resize(children);
            for (int i = 0; i < children; i++)
            {
                ReadBone(bone.children[i], file, level);
            }
        }
    };
};
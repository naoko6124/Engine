#pragma once
#include "pch.hpp"
#include "Renderer/Mesh.hpp"

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
            printf("%lld\n", tsize);
            file.read((char*)data, tsize);

            m.SetTexture(data, width, height, channels);

            delete[] data;

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

            printf("\n%lld", tsize);
            printf("\n%d, %d, %d", width, height, channels);

            file.close();
            return m;
        }
    };
};
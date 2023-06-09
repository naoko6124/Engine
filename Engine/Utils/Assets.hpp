#pragma once
#include "pch.hpp"

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
    };
};
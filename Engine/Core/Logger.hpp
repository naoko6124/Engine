#pragma once
#include "pch.hpp"

namespace Engine
{
    class Logger
    {
    public:
        static void Log(std::string format, ...)
        {
            va_list argptr;
            time_t now = time(0);
            char current[26];
            ctime_s(current, sizeof(current), &now);
            std::string message = "[Log] " + format + ", " + current;
            va_start(argptr, format);
            vprintf(message.c_str(), argptr);
            va_end(argptr);
        }
        static void Warning(std::string format, ...)
        {
            va_list argptr;
            time_t now = time(0);
            char current[26];
            ctime_s(current, sizeof(current), &now);
            std::string message = "[Warning] " + format + ", " + current;
            va_start(argptr, format);
            vprintf(message.c_str(), argptr);
            va_end(argptr);
        }
        static void Error(std::string format, ...)
        {
            va_list argptr;
            time_t now = time(0);
            char current[26];
            ctime_s(current, sizeof(current), &now);
            std::string message = "[Error] " + format + ", " + current;
            va_start(argptr, format);
            vfprintf(stderr, message.c_str(), argptr);
            va_end(argptr);
        }
    };
}
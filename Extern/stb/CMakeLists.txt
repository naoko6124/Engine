cmake_minimum_required(VERSION 3.0)
project(stb)

file (GLOB_RECURSE stb_src
    "include/**.h"
    "include/**.hpp"
    "include/**.c"
    "include/**.cpp"
)
add_library(stb STATIC ${stb_src})
target_include_directories(stb PUBLIC include)
cmake_minimum_required(VERSION 3.0)
project(glm)

file (GLOB_RECURSE glm_src
    "include/**.h"
    "include/**.cpp"
)
add_library(glm STATIC ${glm_src})
target_include_directories(glm PUBLIC include)
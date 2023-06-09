cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0063 NEW)

project(flecs LANGUAGES C)

file (GLOB flecs_src
    "include/flecs.h"
    "src/flecs.c"
)
add_library(flecs ${flecs_src})
target_include_directories(flecs PUBLIC include)
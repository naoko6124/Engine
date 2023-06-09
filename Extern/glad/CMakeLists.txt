cmake_minimum_required(VERSION 3.0)
project(glad)

file (GLOB glad_src
    "include/glad/gl.h"
    "include/glad/vulkan.h"
    "include/KHR/khrplatform.h"
    "include/vk_platform.h"
    "src/gl.c"
    "src/vulkan.c"
)
add_library(glad ${glad_src})
target_include_directories(glad PUBLIC include)
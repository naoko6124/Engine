cmake_minimum_required(VERSION 3.0)
project(json)

file (GLOB_RECURSE json_src
    "include/**.hpp"
    "include/**.cpp"
)
add_library(json STATIC ${json_src})
target_include_directories(json PUBLIC include)
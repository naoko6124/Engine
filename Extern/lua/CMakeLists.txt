cmake_minimum_required(VERSION 3.0)
project(lua)
set(CMAKE_CXX_STANDARD 17)

file (GLOB_RECURSE lua_src src/**.c src/**.cpp src/**.h src/**.hpp)
add_library(lua ${lua_src})
target_include_directories(lua PUBLIC src)
cmake_minimum_required(VERSION 3.0)
project(imgui)

file (GLOB imgui_src src/*.h src/*.cpp)
add_library(imgui ${imgui_src})
target_link_libraries(imgui PUBLIC glfw)
target_include_directories(imgui PUBLIC src)
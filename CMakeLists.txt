cmake_minimum_required (VERSION 3.0.0)

add_subdirectory(extern/assimp)
add_subdirectory(extern/glad)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
add_subdirectory(extern/glfw)
add_subdirectory(extern/glm)
add_subdirectory(extern/stb)

project (Engine)
file (GLOB_RECURSE EngineSrc Engine/**.*)
add_library (Engine STATIC ${EngineSrc})
target_precompile_headers (Engine PUBLIC Engine/pch.hpp)
target_link_libraries (Engine PUBLIC glad glfw glm stb)
target_include_directories (Engine PUBLIC Engine)

project (Game)
file (GLOB_RECURSE GameSrc Game/**.*)
add_executable (Game ${GameSrc})
target_link_libraries (Game PUBLIC Engine)

file (COPY builtin DESTINATION ${PROJECT_BINARY_DIR})

project (Sandbox)
file (GLOB_RECURSE SandboxSrc Sandbox/**.*)
add_executable (Sandbox ${SandboxSrc})
target_link_libraries (Sandbox PUBLIC glm stb)

project (FBX2Mesh)
file (GLOB_RECURSE FBX2MeshSrc FBX2Mesh/**.*)
add_executable (FBX2Mesh ${FBX2MeshSrc})
target_link_libraries (FBX2Mesh PUBLIC assimp glm stb)

project (FBX2SMesh)
file (GLOB_RECURSE FBX2SMeshSrc FBX2SMesh/**.*)
add_executable (FBX2SMesh ${FBX2SMeshSrc})
target_link_libraries (FBX2SMesh PUBLIC assimp glm stb)
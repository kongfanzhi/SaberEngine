# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/external/glfw glfw)

# Glad
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/external/glad glad)

# GLM (header-only)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${CMAKE_SOURCE_DIR}/Engine/external/glm)

# ImGui
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/external/imgui imgui)

# spdlog
add_subdirectory(${CMAKE_SOURCE_DIR}/Engine/external/spdlog spdlog)

# stb_image (header-only)
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${CMAKE_SOURCE_DIR}/Engine/external)

# 添加引擎库
file(GLOB_RECURSE ENGINE_SOURCES 
"${CMAKE_SOURCE_DIR}/Engine/src/Saber/*.cpp" 
"${CMAKE_SOURCE_DIR}/Engine/src/Platform/*.cpp"
"${CMAKE_SOURCE_DIR}/Engine/external/stb_image/stb_image.cpp" )

add_library(SaberEngine STATIC ${ENGINE_SOURCES})

# 添加预编译头文件
target_precompile_headers(SaberEngine PRIVATE src/Saber/pch.h)

target_include_directories(SaberEngine 
    PUBLIC 
    ${CMAKE_SOURCE_DIR}/Engine/src
    ${CMAKE_SOURCE_DIR}/Engine/external/glad/include
    ${CMAKE_SOURCE_DIR}/Engine/external/glfw/include
    ${CMAKE_SOURCE_DIR}/Engine/external/imgui
    ${CMAKE_SOURCE_DIR}/Engine/external
)

target_link_libraries(SaberEngine
    PUBLIC
    glfw
    glad
    glm
    imgui
    spdlog
    stb_image
)

add_definitions(-DGLFW_INCLUDE_NONE) 
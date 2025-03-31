# 添加所有源文件
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE SABER_SOURCES "Saber/*.cpp")

add_executable(${PROJECT_NAME} 
    ${SOURCES}
    ${SABER_SOURCES}
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    glfw
    glad
    glm
    imgui
    spdlog
    stb_image
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Saber
    ${CMAKE_SOURCE_DIR}/external/glad/include
    ${CMAKE_SOURCE_DIR}/external/glfw/include
    ${CMAKE_SOURCE_DIR}/external/imgui
    ${CMAKE_SOURCE_DIR}/external/stb_image
)

# 添加预处理器定义
target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    GLFW_INCLUDE_NONE
)

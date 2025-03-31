file(GLOB_RECURSE SANDBOX_SOURCES "*.cpp")

add_executable(Sandbox ${SANDBOX_SOURCES})



target_include_directories(Sandbox PRIVATE 
${CMAKE_SOURCE_DIR}/Engine/src
${CMAKE_SOURCE_DIR}/Engine/external/imgui
)

target_link_libraries(Sandbox PRIVATE SaberEngine) 

add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/Sandbox/assets
    ${CMAKE_BINARY_DIR}/bin/assets)


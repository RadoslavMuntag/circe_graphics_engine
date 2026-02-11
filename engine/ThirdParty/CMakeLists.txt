add_library(ThirdParty INTERFACE)

target_link_libraries(ThirdParty INTERFACE
    glfw
    glad
    imgui
)

target_include_directories(ThirdParty INTERFACE
    ${CMAKE_SOURCE_DIR}/external/glm
    ${CMAKE_SOURCE_DIR}/external/stb
)

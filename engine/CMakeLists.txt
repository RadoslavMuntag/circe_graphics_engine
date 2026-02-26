add_library(Circe SHARED)

target_sources(Circe
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Core/Log.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Core/Window.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Core/Engine.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Core/Time.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Core/Logging/ErrorReporting.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Renderer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Camera.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Shader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Texture.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Mesh.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Material.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Light.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Renderer/Model.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/SimpleMeshGen.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Scene/Entity.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/Scene/Scene.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/stb.cpp
)

target_precompile_headers(Circe
    PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/pch.h>"
)

# Shared library configuration
target_compile_definitions(Circe 
    PRIVATE 
        CIRCE_BUILD_DLL
        $<$<CONFIG:Debug>:CIRCE_DEBUG>
        $<$<CONFIG:Release>:CIRCE_RELEASE>
)
set_target_properties(Circe PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    VERSION 1.0.0
    SOVERSION 1
)

target_include_directories(Circe
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/external/glm
        ${CMAKE_SOURCE_DIR}/external/stb
        ${CMAKE_SOURCE_DIR}/external/glad/include
        ${CMAKE_SOURCE_DIR}/external/spdlog/include
)

# GLFW
target_link_libraries(Circe
    PRIVATE glfw
)

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(Circe PRIVATE OpenGL::GL)

# Glad
add_library( glad STATIC
    ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c
)

target_include_directories(glad PUBLIC 
    ${CMAKE_SOURCE_DIR}/external/glad/include
)

set_target_properties(glad PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

target_link_libraries(Circe PRIVATE glad)

# GLM


# STB
target_sources(Circe PRIVATE ThirdParty/stb.cpp)


# ImGUI

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

add_library(imgui STATIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

set_target_properties(imgui PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

target_link_libraries(imgui PRIVATE glfw glad)
target_link_libraries(Circe PRIVATE imgui)





# Circe Engine Structure

This document provides a high-level overview of the project layout, modules, and how they relate to each other.

## Top-Level Layout

- `CMakeLists.txt`: Root build configuration.
- `cmake/`: CMake helper scripts.
- `assets/`: Runtime assets (models, textures, shaders, etc.).
- `engine/`: Engine source code.
- `game/`: Example game / application entry point.
- `external/`: Third-party dependencies (GLFW, GLM, ImGui, stb, etc.).
- `build/`: Generated build artifacts (out of source).

## Engine Modules

### Core

Path: `engine/Core/`

- `Engine.*`: Application lifecycle, initialization, and main loop control.
- `Window.*`: Platform window creation and management.
- `Time.*`: Timing utilities and frame delta tracking.
- `Logging/`: Logging helpers (streaming, levels, and sinks if present).

### Math

Path: `engine/Math/`

- `Transform.h`: Transform data (position, rotation, scale) and helpers.

### Platform

Path: `engine/Platform/`

- Platform-specific integrations (windowing, input, filesystem, etc.).

### Renderer

Path: `engine/Renderer/`

- `Renderer.*`: Main rendering pipeline interface.
- `Shader.*`: Shader compilation, linking, and uniform updates.
- `Texture.*`: Texture loading and GPU resource handling.
- `Material.*`: Material properties that bind shaders and textures.
- `Mesh.*`: GPU mesh buffers and draw calls.
- `Model.*`: Model composition (meshes + materials).

### Resources

Path: `engine/Ressources/`

- `ModelLoader.h`: Model import and conversion to engine objects.
- `TextureManager.h`: Texture cache, lifetime, and lookup.

Note: The folder name is spelled `Ressources` in the codebase.

### Scene

Path: `engine/Scene/`

- `Entity.h`: Scene entities and component ownership.
- `Scene.*`: Scene graph, entity storage, and update flow.

### ThirdParty

Path: `engine/ThirdParty/`

- `stb.cpp`: Single translation unit for stb library implementations.

## Game Module

Path: `game/`

- `main.cpp`: Example application entry point using the engine.
- `CMakeLists.txt`: Game target configuration.

## External Dependencies

Path: `external/`

- `glfw/`: Windowing and input library.
- `glad/`: OpenGL loader.
- `glm/`: Math library.
- `imgui/`: Debug UI.
- `stb/`: Image loading and utilities.

## Build Flow (CMake)

- Configure build in `build/` using CMake.
- The `engine/` and `game/` targets are built separately and linked.
- External dependencies are built or included by CMake.

## Notes

- The `build/` directory should not be edited manually.
- Keep assets in `assets/` and load them via engine resource managers where possible.

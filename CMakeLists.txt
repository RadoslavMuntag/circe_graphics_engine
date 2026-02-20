cmake_minimum_required(VERSION 3.20)
project(Circe LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# Compiler flags for different configurations
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Debug flags
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -DCIRCE_DEBUG")
    # Release flags
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    # RelWithDebInfo flags
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Output directories 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Engine and game
add_subdirectory(external/glfw)
add_subdirectory(engine)
add_subdirectory(game)

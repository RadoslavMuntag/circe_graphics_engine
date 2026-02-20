#pragma once

// Platform detection and DLL export/import macros
#ifdef _WIN32
    #ifdef CIRCE_BUILD_DLL
        #define CIRCE_API __declspec(dllexport)
    #else
        #define CIRCE_API __declspec(dllimport)
    #endif
#else
    // For GCC/Clang on Linux/macOS
    #ifdef CIRCE_BUILD_DLL
        #define CIRCE_API __attribute__((visibility("default")))
    #else
        #define CIRCE_API
    #endif
#endif

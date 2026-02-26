add_executable(Game 
    main.cpp
    Behaviors/CameraOrbitBehavior.cpp
    Behaviors/GravityBehavior.cpp
)

target_include_directories(Game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Game PRIVATE Circe)

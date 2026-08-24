cmake_minimum_required(VERSION 3.20)

project(tiny_oni LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tiny_oni
    main.cpp

    core/World.cpp

    camera/Camera.cpp

    rendering/Sky.cpp
    rendering/WorldRenderer.cpp

    persistence/Save.cpp

    ui/UI.cpp
)

target_include_directories(tiny_oni PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    C:/dev/raylib/include
)

target_link_directories(tiny_oni PRIVATE
    C:/dev/raylib/lib
)

target_link_libraries(tiny_oni PRIVATE
    raylib
    winmm
    gdi32
    opengl32
)
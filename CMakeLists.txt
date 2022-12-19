cmake_minimum_required(VERSION 3.24)

project(pgrProject)

find_package(OpenGL REQUIRED)

add_subdirectory(libs)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        assimp
        )

set(SOURCES
        src/main.cpp
        src/model.cpp src/model.h src/GPU.cpp src/GPU.h src/model.cpp)

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)
set(CMAKE_CXX_STANDARD 17)
add_executable(proj ${SOURCES})
target_link_libraries(proj ${ALL_LIBS})

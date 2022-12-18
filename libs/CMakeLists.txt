add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)

### GLFW ###
add_subdirectory(glfw-3.3.8)

include_directories(glfw-3.3.8/include/GLFW/)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(OPENGL_LIBRARY
            ${OPENGL_LIBRARY}
            -lGL -lGLU -lXrandr -lXext -lX11 -lrt
            ${CMAKE_DL_LIBS}
            ${GLFW_LIBRARIES}
            )
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(OPENGL_LIBRARY
            ${OPENGL_LIBRARY}
            ${CMAKE_DL_LIBS}
            ${GLFW_LIBRARIES}
            )
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

### GLM ###
include_directories(glm-0.9.9.8/glm)


## ASSIMP ##
add_subdirectory(assimp-5.2.5)
include_directories(assimp-5.2.5/include/assimp)
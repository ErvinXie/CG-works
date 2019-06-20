cmake_minimum_required(VERSION 3.14)
project(OpenGLtest2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

# 添加头文件
set(GLEW_H /usr/local/Cellar/glew/2.1.0/include/GL)
set(GLFW_H /usr/local/Cellar/glfw/3.3/include/GLFW)
set(INCLUDE_H /usr/local/include)

include_directories(${GLEW_H} ${GLFW_H} ${INCLUDE_H})

# 添加目标链接
set(GLEW_LINK /usr/local/Cellar/glew/2.1.0/lib/libGLEW.2.1.dylib)
set(GLFW_LINK /usr/local/Cellar/glfw/3.3/lib/libglfw.3.dylib)
set(ASSIMP_LINK /usr/local/Cellar/assimp/4.1.0/lib/libassimp.4.1.0.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK} ${ASSIMP_LINK})


add_executable(OpenGLtest2
        shaders/color.frag
        shaders/color.vert
        shaders/lamp.frag
        shaders/lamp.vert

        tools/Camera.cpp
        tools/Shader.cpp

        glad.c
        local_settings.h
        main.cpp
        stb_image.h tools/Light.cpp tools/Material.cpp tools/Material.h tools/Shader.h tools/Light.h tools/Camera.h tools/Mesh.cpp tools/Mesh.h tools/Model.cpp tools/Model.h)

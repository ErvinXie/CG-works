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
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})


add_executable(OpenGLtest2
        shaders/color.frag
        shaders/color.vert
        shaders/lamp.frag
        shaders/lamp.vert
        awesomeface.png
        Camera.cpp
        container.jpg
        glad.c
        local_settings.h
        main.cpp
        Shader.cpp
        stb_image.h)

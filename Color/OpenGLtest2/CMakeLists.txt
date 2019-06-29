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

        class/Camera.cpp
        class/Shader.cpp

        glad.c
        local_settings.h
        main.cpp
        stb_image.h class/Light.cpp class/Material.cpp class/Material.h class/Shader.h class/Light.h class/Camera.h class/Mesh.cpp class/Mesh.h class/Model.cpp class/Model.h class/Physics/Rigid.cpp class/Physics/Rigid.h class/Physics/Sphere.cpp class/Physics/Sphere.h tools/RandMath.h class/Physics/Hitable.cpp class/Physics/Hitable.h class/Physics/Plane.cpp class/Physics/Plane.h)

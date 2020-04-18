//
//  Camera.cpp
//  Color
//
//  Created by 谢威宇 on 2019/4/30.
//  Copyright © 2019 Ervin's Studio. All rights reserved.
//
#ifndef CAMERA_CPP
#define CAMERA_CPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera{
public:
    bool fixedY = 1;//是否固定Y轴永远朝上
    glm::vec3 Position = glm::vec3(0.0f, 1.0f, 3.0f);//摄像机位置
    glm::vec3 Front = glm::vec3(0.0f, -0.3f,1.0f);//摄像机朝向
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);//摄像机顶部向量
    float Fov = 45.0f;//缩放级别
    const float max_fov = 45.0f;//最大缩放级别
    float near_distance = 0.1f;//近裁剪平面
    float far_distance = 2000.0f;//远裁剪平面
    const unsigned int screen_width = 1920;//摄像机画面宽度
    const unsigned int screen_height = 1080;//摄像机画面高度
    float cameraSpeed = 50.0f;//摄像机移动速度

    glm::mat4 View();
    void rotate(float xoffset, float yoffset);
    void rotate(glm::vec3 rotationAxis, float rotateAngel);
    void setFov(float nfov);
    void translate(glm::vec3 tra);
};

#endif

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

class Camera
{
  public:
    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    float Fov = 45.0f;
    float cameraSpeed = 2.5f;
    glm::mat4 View()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }
    void rotate(float xoffset, float yoffset)
    {
        float rotateAngel = sqrt(xoffset * xoffset + yoffset * yoffset);
        if (rotateAngel == 0)
        {
            return;
        }
        glm::vec3 cameraRight = glm::cross(Front, Up);
        glm::vec3 cusorDirection = glm::normalize(xoffset * cameraRight + yoffset * Up);
        glm::vec3 rotationAxis = glm::normalize(glm::cross(cusorDirection, Front));
        rotate(rotationAxis, rotateAngel);
    }

    void rotate(glm::vec3 rotationAxis, float rotateAngel)
    {

        glm::quat ro = glm::angleAxis(glm::radians(rotateAngel), rotationAxis);
        glm::quat cameraFrontq = glm::quat(0, Front);
        cameraFrontq = glm::inverse(ro) * cameraFrontq * ro;
        glm::vec3 cameraFrontn = glm::vec3(cameraFrontq.x, cameraFrontq.y, cameraFrontq.z);
        Front = cameraFrontn;
        glm::quat cameraUpq = glm::quat(0, Up);
        cameraUpq = glm::inverse(ro) * cameraUpq * ro;
        glm::vec3 cameraUpn = glm::vec3(cameraUpq.x, cameraUpq.y, cameraUpq.z);
        Up = cameraUpn;
    }
    void setFov(float nfov)
    {
        if (nfov >= 1.0f && nfov <= 45.0f)
            Fov = nfov;
    }
    void translate(glm::vec3 tra)
    {
        Position += tra;
    }
};
#endif

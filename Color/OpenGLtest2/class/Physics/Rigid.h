//
// Created by 谢威宇 on 2019-06-28.
//

#ifndef OPENGLTEST2_RIGID_H
#define OPENGLTEST2_RIGID_H

#include "glm/glm.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Rigid {
public:
    glm::vec3 center;
    glm::vec3 velocity;

    glm::vec3 omega = glm::vec3(0, 0, 0);

    glm::mat4 roMat = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1, 0, 0));

    float mass;
    float resist = 0.0f;

    Rigid(const glm::vec3 &center);

    Rigid(const glm::vec3 &center, float mass);


    void move(float time);

    void move(glm::vec3 force, float time, glm::vec3 dOmega = glm::vec3(0, 0, 0));
};


#endif //OPENGLTEST2_RIGID_H

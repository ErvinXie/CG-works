//
// Created by 谢威宇 on 2019-06-28.
//

#ifndef OPENGLTEST2_RIGID_H
#define OPENGLTEST2_RIGID_H

#include "glm/glm.hpp"
class Rigid {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    float mass;
    float resist=0.0f;

    Rigid(const glm::vec3 &position, float mass);
    glm::vec3 move(float time);
    glm::vec3 move(glm::vec3 force, float time);
};


#endif //OPENGLTEST2_RIGID_H

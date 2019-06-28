//
// Created by 谢威宇 on 2019-06-28.
//

#include "Rigid.h"

Rigid::Rigid(const glm::vec3 &position, float mass) : position(position), mass(mass) {
    velocity = glm::vec3(0,0,0);
    mass = 1.0f;
}

glm::vec3 Rigid::move(float time) {
    velocity += -velocity*(time*resist);
    position += velocity*time;
}
glm::vec3 Rigid::move(glm::vec3 force, float time) {
    velocity += time*force;
    return move(time);
}




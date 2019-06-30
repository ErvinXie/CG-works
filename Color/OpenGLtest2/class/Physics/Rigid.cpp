//
// Created by 谢威宇 on 2019-06-28.
//

#include "Rigid.h"

Rigid::Rigid(const glm::vec3 &center, float mass) : center(center), mass(mass) {
    velocity = glm::vec3(0, 0, 0);
}

Rigid::Rigid(const glm::vec3 &center) : center(center) {}

void Rigid::move(glm::vec3 force, float time, glm::vec3 dOmega) {
    velocity += time * force;
    omega += time * dOmega;
    return move(time);
}
void Rigid::move(float time) {
    velocity += -velocity * (time * resist);
    center += velocity * time;
    omega += -omega * (time * resist);
    if (glm::length(omega) > 0)
        roMat = glm::rotate(roMat, glm::radians(glm::length(omega)) * time, omega);
}








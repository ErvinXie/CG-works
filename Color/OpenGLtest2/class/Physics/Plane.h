//
// Created by 谢威宇 on 2019-06-29.
//

#ifndef OPENGLTEST2_PLANE_H
#define OPENGLTEST2_PLANE_H

#include "class/Model.h"
#include "class/Camera.h"

class Plane : public Model {
    glm::vec3 center;
    glm::vec3 normal;
public:
    Plane(const glm::vec3 &center, const glm::vec3 &normal);

    void Draw(Shader shader, Camera &camera);
};


#endif //OPENGLTEST2_PLANE_H

//
// Created by 谢威宇 on 2019-06-28.
//

#ifndef OPENGLTEST2_SPHERE_H
#define OPENGLTEST2_SPHERE_H

#include "Rigid.h"
#include "class/Model.h"
#include "class/Camera.h"
#include "Plane.h"
#include "Box.h"


class Sphere :public Rigid,public Model{
public:
    float radius=1.0f;
    float normal_loss = 0.2f;
    float tangent_loss = 0.0f;

    Sphere();
    Sphere(const glm::vec3 &center, float radius,float mass, const string &path);

    bool hitSphere(Sphere& b);
    bool hitPlane(Plane& b);
    bool hitBox(Box& b);

    void velocity_loss(glm::vec3 norm);

    void Draw(Shader shader,Camera& camera) override;
};


#endif //OPENGLTEST2_SPHERE_H

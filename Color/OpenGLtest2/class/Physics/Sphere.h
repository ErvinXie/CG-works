//
// Created by 谢威宇 on 2019-06-28.
//

#ifndef OPENGLTEST2_SPHERE_H
#define OPENGLTEST2_SPHERE_H

#include "Rigid.h"
#include "class/Model.h"
#include "class/Camera.h"

class Sphere :public Rigid,public Model{
public:
    float radius=1.0f;

    float hit_loss = 0.0f;
    Sphere(const glm::vec3 &position, float radius,float mass, const string &path);

    Sphere();

    bool hitSphere(Sphere& b);

    void Draw(Shader shader,Camera& camera);
};


#endif //OPENGLTEST2_SPHERE_H

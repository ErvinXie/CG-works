//
// Created by 谢威宇 on 2019-06-28.
//

#include "Sphere.h"


Sphere::Sphere(const glm::vec3 &position, float radius, float mass, const string &path) : Rigid(position, mass),
                                                                                          radius(radius),
                                                                                          Model(path) {}


bool Sphere::hitSphere(Sphere &b) {
    float sdis = glm::length(this->center - b.center) - (this->radius + b.radius);
    if (sdis <= 0) {
        this->center -= sdis * glm::normalize(this->center - b.center);
        velocity_loss(glm::normalize(b.center - this->center));
        return true;
    } else {
        return false;
    }
}

bool Sphere::hitPlane(Plane &b) {
    float sdis = glm::dot(this->center - b.center, b.normal) - (this->radius);
    if (sdis <= 0) {
        this->center -= sdis * glm::normalize(b.normal);
        velocity_loss(b.normal);
        return true;
    } else {
        return false;
    }
}

bool Sphere::hitBox(Box &b) {


    glm::vec3 v = this->center - b.center;

    v = glm::transpose(b.roMat)*glm::vec4(v,1);

    float sign[3] = {1, 1, 1};
    if (v.x < 0)sign[0] = -1;
    if (v.y < 0)sign[1] = -1;
    if (v.z < 0)sign[2] = -1;

    v = glm::abs(v);
    glm::vec3 h(b.a / 2, b.b / 2, b.c / 2);
    glm::vec3 u = v - h;
    if (u.x < 0)u.x = 0;
    if (u.y < 0)u.y = 0;
    if (u.z < 0)u.z = 0;
    u.x *= sign[0];
    u.y *= sign[1];
    u.z *= sign[2];
    v.x *= sign[0];
    v.y *= sign[1];
    v.z *= sign[2];
    h.x *= sign[0];
    h.y *= sign[1];
    h.z *= sign[2];
    float sdis = glm::length(u) - radius;

    if (sdis <= 0) {
//        cout<<"u: "<<u.x<<" "<<u.y<<" "<<u.z<<endl;
//        cout<<"v: "<<v.x<<" "<<v.y<<" "<<v.z<<endl;
//        cout<<"h: "<<h.x<<" "<<h.y<<" "<<h.z<<endl;

//        this->velocity = glm::vec3(0,0,0);
        if (u == glm::vec3(0, 0, 0)) {
            this->center = b.center + v;
            h = b.roMat*glm::vec4(glm::normalize(h),1);
            velocity_loss(h);

        } else {
            this->center -= sdis * glm::normalize(u);
            u = b.roMat*glm::vec4(glm::normalize(u),1);
            velocity_loss(u);
        }
        return true;
    } else {
        return false;
    }
}

Sphere::Sphere() : Rigid(glm::vec3(0, 0, 0), 1.0),

                   Model(/*这里很奇怪，如果引入local_settings会导致link error*/"../objects/sphere/sphere.obj") {
    this->radius = 1.0;
}

void Sphere::Draw(Shader shader, Camera &camera) {

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Fov),
                                            (float) camera.screen_width / (float) camera.screen_height,
                                            camera.near_distance, camera.far_distance);
    glm::mat4 view = camera.View();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->center);

    model *= roMat;

    model = glm::scale(model, glm::vec3(1, 1, 1) * this->radius);

    shader.setMat4("model", model);
    Model::Draw(shader);
}

void Sphere::velocity_loss(glm::vec3 norm) {
    this->velocity = glm::reflect(this->velocity, norm);
    glm::vec3
            nv = glm::dot(this->velocity, norm) * norm,
            tv = this->velocity - nv;

//    std::cout<<"omega:"<<omega.x<<" "<<omega.y<<" "<<omega.z<<endl;

    this->omega += glm::length(tv)*glm::cross(nv,tv);

    this->velocity = (1 - normal_loss) * nv + (1 - tangent_loss) * tv;

}





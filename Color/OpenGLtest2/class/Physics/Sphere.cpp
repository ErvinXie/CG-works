//
// Created by 谢威宇 on 2019-06-28.
//

#include "Sphere.h"

Sphere::Sphere(const glm::vec3 &position, float radius, float mass, const string &path) : Rigid(position, mass),
                                                                                          radius(radius),
                                                                                          Model(path) {}

bool Sphere::hitSphere(Sphere &b) {
    float sdis = glm::length(this->position - b.position) - (this->radius + b.radius);
    if (sdis <= 0) {
        this->position -= sdis * glm::normalize(this->position - b.position);
        glm::vec3 normal = glm::normalize(b.position - this->position);
        this->velocity = glm::reflect(this->velocity, normal);
        this->velocity -= hit_loss * glm::dot(this->velocity, normal) * normal;
        return true;
    } else {
        return false;
    }
}

Sphere::Sphere() : Rigid(glm::vec3(0, 0, 0), 1.0),

                   Model(/*这里很奇怪，如果引入local_settings会导致link error*/"../objects/sphere/sphere.obj") {
    this->radius = 1.0;
}

void Sphere::Draw(Shader shader,Camera& camera) {

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), (float) camera.screen_width / (float) camera.screen_height,
                                            camera.near_distance, camera.far_distance);
    glm::mat4 view = camera.View();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->position);
    model = glm::scale(model, glm::vec3(1, 1, 1) * this->radius);

    shader.setMat4("model", model);
    Model::Draw(shader);
}

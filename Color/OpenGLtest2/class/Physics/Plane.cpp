//
// Created by 谢威宇 on 2019-06-29.
//

#include "Plane.h"

Plane::Plane(const glm::vec3 &center,const glm::vec3 &normal) : Model("../objects/ground/ground.obj"),
                                                                 center(center),
                                                                 normal(normal) {
    this->normal = glm::normalize(normal);

}

void Plane::Draw(Shader shader, Camera& camera) {
    glm::mat4 projection = glm::perspective(glm::radians(camera.Fov),
                                            (float) camera.screen_width / (float) camera.screen_height,
                                            camera.near_distance, camera.far_distance);
    glm::mat4 view = camera.View();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, center);

    glm::vec3 axis = glm::cross(glm::vec3(0,1,0),normal);
    if(axis==glm::vec3(0,0,0)){
        axis = glm::vec3(1,0,0);
    }
    model = glm::rotate(model,glm::angle(glm::vec3(0,1,0),normal),axis);
    model = glm::scale(model, glm::vec3(1, 1, 1));

    shader.setMat4("model", model);
    Model::Draw(shader);
}

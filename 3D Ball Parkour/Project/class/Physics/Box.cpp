//
// Created by 谢威宇 on 2019-06-29.
//

#include <tools/RandMath.h>
#include "Box.h"


Box::Box() :
        Model("../objects/box/box.obj"),
        Rigid(glm::vec3(0, 0, 0)),
        a(1), b(1),
        c(1) {}

Box::Box(const glm::vec3 &center) :
        Model("../objects/box/box.obj"),
        Rigid(center),
        a(1), b(1),
        c(1) {}

Box::Box(const glm::vec3 &center, float a, float b, float c) :
        Model("../objects/box/box.obj"),
        Rigid(center),
        a(a), b(b),
        c(c) {}
Box::Box(const glm::vec3 &center, float a, float b, float c,string path) :
        Model(path.c_str()),
        Rigid(center),
        a(a), b(b),
        c(c) {}

void Box::Draw(Shader shader, Camera &camera) {
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
    model = glm::scale(model, 0.5f * glm::vec3(a, b, c));
    shader.setMat4("model", model);
    Model::Draw(shader);

}

Box Box::getNext(float difficult) {

    float ut = max(200.0 / (difficult + 100), 1.0) * 6,
            dt = max(200.0 / (difficult + 100), 1.0) * 2;
    if ((int) difficult % 7 == 0) {
        ut*=2;
    }


    float a = RAND(ut, dt);
    float c = RAND(ut, dt);

    float dx = RAND((this->a + a), 10);
    float dz = RAND((this->c + c), 10);

    if (RAND(0, 1) < 0.5) {
        dx = -dx;
    }
//    if(RAND(0,1)<0.5){
//        dz=-dz;
//    }

    float b = RAND(0.5, 20);
    float dy = RAND(-2, 2) + (this->b - b) / 2;


    dt *= 5;
    Box n(glm::vec3(dx, dy, dz) + center, a, b, c);
    if (difficult >= 1 && (int) difficult % 7 != 0) {
        n.roMat = glm::rotate(n.roMat, (float) glm::radians(RAND(-dt, dt)),
                              glm::vec3(RAND(0.0f, 1.0f), RAND(0,1.0f), RAND(0, 1.0f)));
    }
    return n;
}



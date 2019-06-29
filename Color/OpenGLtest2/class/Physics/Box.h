//
// Created by 谢威宇 on 2019-06-29.
//

#ifndef OPENGLTEST2_BOX_H
#define OPENGLTEST2_BOX_H


#include <class/Model.h>
#include "Rigid.h"

class Box : public Model, public Rigid {
public:
    float a = 1, b = 1, c = 1;

    Box();
    Box(const glm::vec3 &center);
    Box(const glm::vec3 &center, float a, float b, float c);

    Box getNext(float difficult);
    void Draw(Shader shader, Camera &camera) override;

};




#endif //OPENGLTEST2_BOX_H

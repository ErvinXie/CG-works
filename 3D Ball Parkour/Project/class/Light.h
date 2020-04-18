//
// Created by 谢威宇 on 2019-06-20.
//

#ifndef LIGHT_H
#define LIGHT_H

#include "Shader.h"


class Light {
public:
    float light_strength = 1.0f;
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

    virtual std::string getName(int i) = 0;

    virtual void setShader(Shader &shader, int i);
};


class DirLight : public Light {
public:
    glm::vec3 direction;
    DirLight(const glm::vec3 &direction) : direction(direction) {}
    std::string getName(int i) override { return "dirLight"; }
    void setShader(Shader &shader, int i) override;
};

class AttenuateLight : public Light {
public:
    float constant = 1.0f;
    float linear = 0.045f;
    float quadratic = 0.0075f;

    void setShader(Shader &shader, int i) override;
};

class PointLight : public AttenuateLight {
public:
    glm::vec3 position;

    PointLight(const glm::vec3 &position) : position(position) {}

    std::string getName(int i) override {
        return "pointLights[" + std::to_string(i) + "]";
    }

    void setShader(Shader &shader, int i) override;

};

class SpotLight : public AttenuateLight {
public:
    glm::vec3 position;
    glm::vec3 direction;

    float cutOff = 12.5f;
    float outerCutOff = 17.5f;

    SpotLight(const glm::vec3 &position, const glm::vec3 &direction) : position(position), direction(direction) {}

    SpotLight(const glm::vec3 &position, const glm::vec3 &direction, float cutOff, float outerCutOff) : position(
            position), direction(direction), cutOff(cutOff), outerCutOff(outerCutOff) {}

    std::string getName(int i) override {
        return "spotLights[" + std::to_string(i) + "]";
    }

    void setShader(Shader &shader, int i) override;
};

#endif
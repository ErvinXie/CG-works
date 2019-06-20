#include "Light.h"
void Light::setShader(Shader &shader, int i) {
    std::string name = getName(i);
//        printf("%s\n",name.c_str());
    shader.setVec3(name + ".ambient", ambient);
    shader.setVec3(name + ".diffuse", diffuse);
    shader.setVec3(name + ".specular", specular);
}

void DirLight::setShader(Shader &shader, int i) {
    Light::setShader(shader, i);
    shader.setVec3(getName(i) + ".direction", direction);
}

void AttenuateLight::setShader(Shader &shader, int i) {
    Light::setShader(shader, i);
    std::string name = getName(i);
    shader.setFloat(name + ".constant", constant);
    shader.setFloat(name + ".linear", linear);
    shader.setFloat(name + ".quadratic", quadratic);
}

void PointLight::setShader(Shader &shader, int i) {
    AttenuateLight::setShader(shader, i);
    shader.setVec3(getName(i) + ".position", position);
}

void SpotLight::setShader(Shader &shader, int i) {
    AttenuateLight::setShader(shader, i);
    std::string name = getName(i);
    shader.setVec3(name + ".position", position);
    shader.setVec3(name + ".direction", direction);
    shader.setFloat(name + ".cutOff", cosf(glm::radians(cutOff)));
    shader.setFloat(name + ".outerCutOff", cosf(glm::radians(outerCutOff)));
}





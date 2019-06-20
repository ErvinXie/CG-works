//
// Created by 谢威宇 on 2019-06-20.
//

#ifndef OPENGLTEST2_MATERIAL_H
#define OPENGLTEST2_MATERIAL_H


unsigned int loadTexture(const char *path);

class Material {
public:
    unsigned int diffuseMap;
    unsigned int specularMap;
    float shininess=32.0f;
    Material(char const *path1, char const *path2);

    void setShader();

};



#endif //OPENGLTEST2_MATERIAL_H

#pragma once
#include <glm/glm.hpp>

class Material {
public:
    glm::vec3 ra;
    glm::vec3 rd;
    glm::vec3 rs;
    float h;

    Material();
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
};
#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

class Light {
public:
    int type;
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 atten;
    glm::vec3 direction;
    float cutoff;

    Light(int typ, glm::vec3 pos, glm::vec3 color, glm::vec3 atte);
    Light(glm::vec3 pos, glm::vec3 color, glm::vec3 att);
    Light(glm::vec3 pos, glm::vec3 dir, glm::vec3 col, float cut, glm::vec3 att);
};

#endif
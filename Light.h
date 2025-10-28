#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

class Light {
public:
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 atten;

    Light(glm::vec3 pos, glm::vec3 color, glm::vec3 att);
};

#endif
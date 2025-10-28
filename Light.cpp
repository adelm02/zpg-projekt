#include "Light.h"

Light::Light(glm::vec3 pos, glm::vec3 col, glm::vec3 att) {

    position = pos;
    color = col;
    atten = att;
}
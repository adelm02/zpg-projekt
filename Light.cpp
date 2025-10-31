#include "Light.h"

#include "Light.h"

Light::Light(glm::vec3 pos, glm::vec3 color, glm::vec3 att)
    : type(1), position(pos), color(color), atten(att) {}

Light::Light(int t, glm::vec3 pos, glm::vec3 col, glm::vec3 att)
    : type(t), position(pos), color(col), atten(att) {}
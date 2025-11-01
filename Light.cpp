#include "Light.h"

Light::Light(glm::vec3 pos, glm::vec3 color, glm::vec3 att)
    : type(1), position(pos), color(color), atten(att), direction(0.0f), cutoff(0.0f) {}

Light::Light(int t, glm::vec3 pos, glm::vec3 col, glm::vec3 att)
    : type(t), position(pos), color(col), atten(att), direction(0.0f), cutoff(0.0f) {}

Light::Light(glm::vec3 pos, glm::vec3 dir, glm::vec3 col, float cut, glm::vec3 att)
    : type(2), position(pos), direction(dir), color(col), cutoff(cut), atten(att) {}
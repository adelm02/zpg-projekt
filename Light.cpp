#include "Light.h"

Light::Light() {

    position = glm::vec3(0.0f, 0.0f, 0.0f);
    ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    specular = glm::vec3(1.0f, 1.0f, 1.0f);
}
#include "Material.h"

Material::Material() {
    ra = glm::vec3(0.1f);
    rd = glm::vec3(0.8f);
    rs = glm::vec3(0.5f);
    h = 32.0f;
}

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) {
    ra = ambient;
    rd = diffuse;
    rs = specular;
    h = shininess;
}
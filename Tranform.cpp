//
// Created by Adéla Martynková on 04.10.2025.
//

#include "Tranform.h"


glm::mat4 Tranform::getMatrix() const {
    return glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Tranform::apply() {
}

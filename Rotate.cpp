//
// Created by Adéla Martynková on 04.10.2025.
//

#include "Rotate.h"

glm::mat4 Rotate::getMatrix() const {
    return glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
}

void Rotate::apply() {
}

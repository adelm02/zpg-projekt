
#include "Scale.h"

glm::mat4 Scale::getMatrix() const {
    return glm::scale(glm::mat4(1.0f), glm::vec3(x, y, z));
}

void Scale::apply() {

}

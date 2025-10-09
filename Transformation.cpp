#include "Transformation.h"

void Transformation::addTrans(AbstractTrans* trans) {
    transformations.push_back(trans);
}

glm::mat4 Transformation::getMatrix() const {
    glm::mat4 result = glm::mat4(1.0f);

    for (auto trans : transformations) {
        result = trans->getMatrix() * result;
    }
    return result;
}

void Transformation::apply() {
}

void Transformation::update(float dt) {
    for (auto *trans : transformations) {
        if (trans) trans->update(dt);
    }
}

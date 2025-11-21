//
// CustomTransform.h
// Vlastní transformace s upravenou homogenní složkou
//

#ifndef ZPGPROJ_CUSTOMTRANSFORM_H
#define ZPGPROJ_CUSTOMTRANSFORM_H

#include "AbstractTrans.h"
#include <glm/gtc/matrix_transform.hpp>

class CustomTransform : public AbstractTrans {
public:
    float customW;

    CustomTransform(float w = 20.0f) : customW(w) {}

    glm::mat4 getMatrix() const override {

        glm::mat4 matrix = glm::mat4(1.0f);

        matrix[3][3] = customW;

        return matrix;
    }

    void apply() override {}

    void setW(float w) {
        customW = w;
    }
};

#endif //ZPGPROJ_CUSTOMTRANSFORM_H
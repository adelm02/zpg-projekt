//
// Created by Adéla Martynková on 04.10.2025.
//

#ifndef ZPGPROJ_TRANFORM_H
#define ZPGPROJ_TRANFORM_H
#include "AbstractTrans.h"
#include <glm/gtc/matrix_transform.hpp>


class Tranform : public AbstractTrans{
    public:
    float x, y, z;

    Tranform(float x, float y, float z)
        :x(x), y(y), z(z) {}

    glm::mat4 getMatrix() const override;
    void apply() override;

};


#endif //ZPGPROJ_TRANFORM_H
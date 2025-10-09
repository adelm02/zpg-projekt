//
// Created by Adéla Martynková on 04.10.2025.
//

#ifndef ZPGPROJ_SCALE_H
#define ZPGPROJ_SCALE_H
#include "AbstractTrans.h"
#include <glm/gtc/matrix_transform.hpp>


class Scale : public AbstractTrans{
    public:
    float x,y,z;
    Scale(float x, float y, float z) : x(x), y(y), z(z){};
    glm::mat4 getMatrix() const override;
    void apply() override;

};


#endif //ZPGPROJ_SCALE_H
//
// Created by Adéla Martynková on 04.10.2025.
//

#ifndef ZPGPROJ_ROTATE_H
#define ZPGPROJ_ROTATE_H
#include "AbstractTrans.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>


class Rotate : public AbstractTrans{
    public:
    float angle;
    glm::vec3 axis;
    Rotate(float angle, glm::vec3 axis):angle (angle), axis(axis) {};
    glm::mat4 getMatrix() const override;
    float speed = 0.0f;
    void apply() override;
    void setSpeed(float s) { speed = s; }
    void update(float dt) override{ angle += speed * dt;}

};


#endif //ZPGPROJ_ROTATE_H
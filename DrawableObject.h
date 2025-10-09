#ifndef ZPGPROJ_DRAWABLEOBJECT_H
#define ZPGPROJ_DRAWABLEOBJECT_H

#include "Modell.h"
#include "ShaderProgram.h"
#include "Transformation.h"
#include <glm/gtc/type_ptr.hpp>

class DrawableObject {
private:
    const Modell& model;
    ShaderProgram& shaderProgram;
    Transformation& transformation;

public:
    DrawableObject(const Modell& model, ShaderProgram& shaderProgram, Transformation& transformation);

    void draw();
    void update(float dt);
};

#endif //ZPGPROJ_DRAWABLEOBJECT_H
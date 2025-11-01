#ifndef ZPGPROJ_DRAWABLEOBJECT_H
#define ZPGPROJ_DRAWABLEOBJECT_H

#include "Modell.h"
#include "ShaderProgram.h"
#include "Transformation.h"
#include <glm/gtc/type_ptr.hpp>
#include "Material.h"

class DrawableObject {
private:
    const Modell& model;
    ShaderProgram& shaderProgram;
    Transformation& transformation;
    Material material;

public:
    DrawableObject(const Modell& model, ShaderProgram& shaderProgram, Transformation& transformation, glm::vec3 color);
    DrawableObject(const Modell& model, ShaderProgram& shaderProgram, Transformation& transformation);


    void draw();
    void update(float dt);
    void setMaterial(const Material& m) { material = m; }
    glm::vec3 color = glm::vec3(0.385f, 0.647f, 0.812f);
};

#endif //ZPGPROJ_DRAWABLEOBJECT_H
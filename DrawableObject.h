
#ifndef ZPGPROJ_DRAWABLEOBJECT_H
#define ZPGPROJ_DRAWABLEOBJECT_H

#include "Modell.h"
#include "ShaderProgram.h"
#include "Transformation.h"
#include "Texture.h"
#include "Material.h"
#include <glm/gtc/type_ptr.hpp>

class DrawableObject {
private:
    const Modell& model;
    ShaderProgram& shaderProgram;
    Transformation& transformation;
    Material material;
    Texture* texture;
    int stencilID; // stencil buffer (0-255)

public:
    DrawableObject(const Modell& model, ShaderProgram& shaderProgram, Transformation& transformation, glm::vec3 color);
    DrawableObject(const Modell& model, ShaderProgram& shaderProgram, Transformation& transformation);

    void draw();
    void drawWithStencil(int stencilValue);
    void update(float dt);
    void setMaterial(const Material& m) { material = m; }
    void setTexture(Texture* tex) { texture = tex; }
    void setStencilID(int id) { stencilID = id; }
    int getStencilID() const { return stencilID; }

    glm::vec3 color = glm::vec3(0.385f, 0.647f, 0.812f);
};

#endif //ZPGPROJ_DRAWABLEOBJECT_H
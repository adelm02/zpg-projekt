#include "DrawableObject.h"
#include <glm/gtc/type_ptr.hpp>

DrawableObject::DrawableObject(const Modell& model, ShaderProgram& shaderProgram, Transformation& transformation)
    : model(model), shaderProgram(shaderProgram), transformation(transformation) {}

void DrawableObject::draw()
{
    if (shaderProgram.useShaderProgram()) {

        glm::mat4 modelMatrix = transformation.getMatrix();
        // Poslat do shaderu
        shaderProgram.SetUniform("model", modelMatrix);

        model.draw();
    }
}

void DrawableObject::update(float dt) {
    transformation.update(dt);
}

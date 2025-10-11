#include "DrawableObject.h"
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"

DrawableObject::DrawableObject(const Modell& model, ShaderProgram& shaderProgram, Transformation& transformation)
    : model(model), shaderProgram(shaderProgram), transformation(transformation) {}

void DrawableObject::draw()
{
    if (shaderProgram.useShaderProgram()) {

        glm::mat4 modelMatrix = transformation.getMatrix();
        // Poslat do shaderu
        shaderProgram.SetUniform("model", modelMatrix);
        shaderProgram.SetUniform("viewMatrix", Camera::getInstance()->getCamera());
        shaderProgram.SetUniform("projectionMatrix", Camera::getInstance()->getProjection() );
        shaderProgram.SetUniform("viewPos", Camera::getInstance()->getCamera());

        model.draw();
    }
}

void DrawableObject::update(float dt) {
    transformation.update(dt);
}

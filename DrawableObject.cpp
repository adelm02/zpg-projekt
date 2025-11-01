#include "DrawableObject.h"
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"

DrawableObject::DrawableObject(const Modell &model, ShaderProgram &shaderProgram, Transformation &transformation,
    glm::vec3 color)
    : model(model), shaderProgram(shaderProgram), transformation(transformation), color(color), material() {}


DrawableObject::DrawableObject(const Modell& model, ShaderProgram& shaderProgram, Transformation& transformation)
    : model(model), shaderProgram(shaderProgram), transformation(transformation), material() {}

void DrawableObject::draw()
{
    shaderProgram.useShaderProgram();

    glm::mat4 modelMatrix = transformation.getMatrix();
    shaderProgram.SetUniform("model", modelMatrix);
    shaderProgram.SetUniform("viewMatrix", Camera::getInstance()->getCamera());
    shaderProgram.SetUniform("projectionMatrix", Camera::getInstance()->getProjection());
    shaderProgram.SetUniform("viewPos", Camera::getInstance()->getCameraPos());
    shaderProgram.SetUniform("objectColor", color);
    
    shaderProgram.SetUniform("material.ra", material.ra);
    shaderProgram.SetUniform("material.rd", material.rd);
    shaderProgram.SetUniform("material.rs", material.rs);
    shaderProgram.SetUniform("material.h", material.h);

    model.draw();
}

void DrawableObject::update(float dt) {
    transformation.update(dt);
}

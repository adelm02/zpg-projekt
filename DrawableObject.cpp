#include "DrawableObject.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

DrawableObject::DrawableObject(const Modell& model, ShaderProgram& shaderProgram, Transformation& transformation, glm::vec3 color)
    : model(model), shaderProgram(shaderProgram), transformation(transformation), color(color), texture(nullptr), stencilID(0) {
    this->material = Material();
    this->material.rd = color;
}

DrawableObject::DrawableObject(const Modell& model, ShaderProgram& shaderProgram, Transformation& transformation)
    : model(model), shaderProgram(shaderProgram), transformation(transformation), texture(nullptr), stencilID(0) {
    this->material = Material();
    this->color = material.rd;
}

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

    if (texture != nullptr && model.getHasTexCoords()) {
        glActiveTexture(GL_TEXTURE0);
        texture->bind(0);
        shaderProgram.SetUniform("diffuseTexture", 0);
        shaderProgram.SetUniform("useTexture", true);
    } else {
        shaderProgram.SetUniform("useTexture", false);
    }

    model.draw();
}

void DrawableObject::drawWithStencil(int stencilValue)
{


    // write to stencil buffer
    glStencilFunc(GL_ALWAYS, stencilValue, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);

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

    if (texture != nullptr && model.getHasTexCoords()) {
        glActiveTexture(GL_TEXTURE0);
        texture->bind(0);
        shaderProgram.SetUniform("diffuseTexture", 0);
        shaderProgram.SetUniform("useTexture", true);
    } else {
        shaderProgram.SetUniform("useTexture", false);
    }

    model.draw();
}

void DrawableObject::update(float dt) {
    transformation.update(dt);
}
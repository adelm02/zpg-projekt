#include "ShaderProgram.h"
#include <iostream>
#include <ostream>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"


ShaderProgram::ShaderProgram() {
    Camera::getInstance()->addObserver(this);
    id = glCreateProgram();
}

ShaderProgram::~ShaderProgram() =default;

bool ShaderProgram::useShaderProgram() const{
    glUseProgram(id);
    return true;
}

void ShaderProgram::addShader(Shader& shader) {
    shader.attachToShader(id);
}

void ShaderProgram::link() {
    glLinkProgram(id);
    GLint status;
    glGetProgramiv(id, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint infoLogLength;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(id, infoLogLength, NULL, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
    }
}

void ShaderProgram::SetUniform(const char *name, const glm::mat4 &matrix) const {
    GLint loc = glGetUniformLocation(id, name);
    if (loc != -1) {
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

void ShaderProgram::SetUniform(const char *name, const glm::vec3 &vector) const {
    GLint loc = glGetUniformLocation(id, name);
    if (loc != -1) {
        glUniform3fv(loc, 1, glm::value_ptr(vector));
    }
}

void ShaderProgram::SetUniform(const char *name, float value) const {
    GLint loc = glGetUniformLocation(id, name);
    if (loc != -1) {
        glUniform1f(loc, value);
    }
}

void ShaderProgram::SetUniform(const char *name, int value) const {
    GLint loc = glGetUniformLocation(id, name);
    if (loc != -1) {
        glUniform1i(loc, value);
    }
}

void ShaderProgram::update() {
    this->useShaderProgram();
    Camera* cam = Camera::getInstance();
    this->SetUniform("viewMatrix", cam->getCamera());
    this->SetUniform("viewPos", cam->getCameraPos());
    this->SetUniform("projectionMatrix", cam->getProjection());

}

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

void ShaderProgram::useShaderProgram() const{
    glUseProgram(id);
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
    glUseProgram(id);
    GLint loc = glGetUniformLocation(id, name);
    if (loc != -1) {
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

void ShaderProgram::SetUniform(const char *name, const glm::vec3 &vector) const {
    glUseProgram(id);
    GLint loc = glGetUniformLocation(id, name);
    if (loc != -1) {
        glUniform3fv(loc, 1, glm::value_ptr(vector));
    }
}

void ShaderProgram::SetUniform(const char *name, float value) const {
    glUseProgram(id);
    GLint loc = glGetUniformLocation(id, name);
    if (loc != -1) {
        glUniform1f(loc, value);
    }
}

void ShaderProgram::SetUniform(const char *name, int value) const {
    glUseProgram(id);
    GLint loc = glGetUniformLocation(id, name);
    if (loc != -1) {
        glUniform1i(loc, value);
    }
}

void ShaderProgram::SetUniformArray3(const char *name, const glm::vec3 *data, int count) const {

    if (!name || !data || count <= 0) return;
    glUseProgram(id);

    std::string indexed0 = std::string(name) + "[0]";
    GLint loc = glGetUniformLocation(id, indexed0.c_str());
    if (loc != -1) {
        glUniform3fv(loc, count, glm::value_ptr(data[0]));
    } else {
        fprintf(stderr, "Uniform array %s not found (expected %s)\n", name, indexed0.c_str());
    }
}

void ShaderProgram::SetUniformArray3(const char *name, const std::vector<glm::vec3> &data, int count) const {
    if (count > (int)data.size()) count = (int)data.size();
    SetUniformArray3(name, data.data(), count);
}

void ShaderProgram::update() {
    this->useShaderProgram();
    Camera* cam = Camera::getInstance();
    this->SetUniform("viewMatrix", cam->getCamera());
    this->SetUniform("viewPos", cam->getCameraPos());
    this->SetUniform("projectionMatrix", cam->getProjection());

}

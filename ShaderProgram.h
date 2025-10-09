#ifndef ZPGPROJ_SHADERPROGRAM_H
#define ZPGPROJ_SHADERPROGRAM_H
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <glm/fwd.hpp>

#include "Shader.h"
class ShaderProgram {
protected:
    GLuint id;
public:
    ShaderProgram();
    ~ShaderProgram();
    bool useShaderProgram ()const;
    void addShader(Shader& shader);
    void link();
    void SetUniform(const char* name, const glm::mat4& matrix) const;
    void SetUniform(const char* name, const glm::vec3& vector) const;
    void SetUniform(const char* name, float value) const;
    void SetUniform(const char* name, int value) const;
};
#endif //ZPGPROJ_SHADERPROGRAM_H
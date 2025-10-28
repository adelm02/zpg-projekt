#ifndef ZPGPROJ_SHADERPROGRAM_H
#define ZPGPROJ_SHADERPROGRAM_H
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <glm/fwd.hpp>

#include "Shader.h"
class ShaderProgram : public Observer{
protected:
    GLuint id;
public:
    ShaderProgram();
    ~ShaderProgram();
    void useShaderProgram ()const;
    void addShader(Shader& shader);
    void link();
    void SetUniform(const char* name, const glm::mat4& matrix) const;
    void SetUniform(const char* name, const glm::vec3& vector) const;
    void SetUniform(const char* name, float value) const;
    void SetUniform(const char* name, int value) const;
    void SetUniformArray3(const char* name, const glm::vec3* data, int count) const;
    void SetUniformArray3(const char* name, const std::vector<glm::vec3>& data, int count) const;
    virtual void update();

};
#endif //ZPGPROJ_SHADERPROGRAM_H
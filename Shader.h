#ifndef ZPGPROJ_SHADER_H
#define ZPGPROJ_SHADER_H
#include <cstring>
#include <GL/glew.h>
#include <stdio.h>
#include <cstring>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include "Observer.h"


class Shader {
public:
    Shader() = default;
    Shader(const char* src, char * type);
    void Compile();
    void attachToShader(GLuint programId);
    void createShader(GLenum shaderType, const char* shaderCode);
    void createShaderFromFile(GLenum shaderType, const char* shaderFile);

protected:
    const char* source = nullptr;
    const char* type   = nullptr;
    GLuint id = 0;

};


#endif //ZPGPROJ_SHADER_H
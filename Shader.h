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
    Shader(const char* src, char * type);
    void Compile();
    void AttachToShader(GLuint programId);

protected:
    const char* source = nullptr;
    const char* type   = nullptr;
    GLuint id = 0;

};


#endif //ZPGPROJ_SHADER_H
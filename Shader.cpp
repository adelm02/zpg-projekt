#include "Shader.h"

#include <fstream>
#include <iostream>


Shader::Shader(const char *src,char* t) {
    source = src;
    type = t;
}

void Shader::Compile(){

    if (std::strcmp(type, "vertex") == 0) {
        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(vertex_shader, 1, &source, NULL);
        glCompileShader(vertex_shader);
        //kontola vertex shaderu
        GLint success;
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (success == 0) {
            char info[512];
            glGetShaderInfoLog(vertex_shader, 512, NULL, info);
            printf("Vertex shader compile error:\n%s\n", info);
        }
        id = vertex_shader;
    }
    if (std::strcmp(type, "fragment") == 0) {
        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(fragment_shader, 1, &source, NULL);
        glCompileShader(fragment_shader);
        GLint success2;
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success2);
        if (success2 == 0) {
            char info2[256];
            glGetShaderInfoLog(fragment_shader, 256, NULL, info2);
            printf("Fragment shader compile error:\n%s\n", info2);
        }
        id = fragment_shader;
    }

}

void Shader::attachToShader(GLuint programId) {

        glAttachShader(programId, id);
}

void Shader::createShader(GLenum shaderType, const char *shaderCode)
{
    // Creates an empty shader
    id = glCreateShader(shaderType);
    glShaderSource(id, 1, &shaderCode, NULL);
    glCompileShader(id);

    GLint success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (success == 0)
    {
        char infoLog[512];
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        std::cout << "Shader compile error:\n" << infoLog << std::endl;
    }
}

void Shader::createShaderFromFile(GLenum shaderType, const char* shaderFile)
{
    //Loading the contents of a file into a variable
    std::ifstream file(shaderFile);
    if (!file.is_open())
    {
        std::cout << "Unable to open file " << shaderFile << std::endl;
        exit(-1);
    }
    std::string shaderCode((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());

    createShader(shaderType, shaderCode.c_str());

}



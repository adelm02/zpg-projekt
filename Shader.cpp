#include "Shader.h"


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

void Shader::AttachToShader(GLuint programId) {

        glAttachShader(programId, id);
}


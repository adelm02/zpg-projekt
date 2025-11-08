#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class ShaderProgram;

class SkyBox {
private:
    GLuint VAO, VBO;
    GLuint textureID;
    ShaderProgram* shaderProgram;

    void setupMesh();
    GLuint loadCubemap(const std::vector<std::string>& faces);

public:
    SkyBox(const std::vector<std::string>& faces, ShaderProgram* program);
    ~SkyBox();
    
    void draw(const glm::mat4& view, const glm::mat4& projection);
};
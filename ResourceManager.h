#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>
#include <map>
#include "Modell.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Texture.h"

class ResourceManager {
private:
    std::map<std::string, Modell*> models;
    std::map<std::string, ShaderProgram*> shaderPrograms;
    std::map<std::string, Shader*> shaders;
    std::map<std::string, Texture*> textures;

public:
    ResourceManager();
    ~ResourceManager();

    // model loading
    void loadModel(const std::string& name, const float* data, int dataSize, int stride);
    void loadModelOBJ(const std::string& name, const std::string& path);
    void loadModelWithTexCoords(const std::string& name, const float* data, int dataSize, int stride);

    // loading shaders
    void loadShader(const std::string& name, GLenum type, const std::string& path);
    void loadShaderProgram(const std::string& name, 
                          const std::string& vertName, 
                          const std::string& fragName);


    void loadTexture(const std::string& name, const std::string& path);

    //getting resiurce
    Modell* getModel(const std::string& name);
    ShaderProgram* getShaderProgram(const std::string& name);
    Texture* getTexture(const std::string& name);
};

#endif
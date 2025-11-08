#include "ResourceManager.h"
#include <iostream>

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {
    //cleanup resources
    for (auto& pair : models) delete pair.second;
    for (auto& pair : shaderPrograms) delete pair.second;
    for (auto& pair : shaders) delete pair.second;
    for (auto& pair : textures) delete pair.second;
}

void ResourceManager::loadModel(const std::string& name, const float* data, int dataSize, int stride) {
    Modell* model = new Modell();
    model->loadData(data, dataSize, stride);
    models[name] = model;
}

void ResourceManager::loadModelOBJ(const std::string& name, const std::string& path) {
    Modell* model = new Modell();
    if (model->loadOBJ(path)) {
        models[name] = model;
    } else {
        delete model;
    }
}

void ResourceManager::loadModelWithTexCoords(const std::string& name, const float* data, int dataSize, int stride) {
    Modell* model = new Modell();
    model->loadDataWithTexCoords(data, dataSize, stride);
    models[name] = model;
}

void ResourceManager::loadShader(const std::string& name, GLenum type, const std::string& path) {
    Shader* shader = new Shader();
    shader->createShaderFromFile(type, path.c_str());
    shaders[name] = shader;
}

void ResourceManager::loadShaderProgram(const std::string& name, const std::string& vertName, const std::string& fragName) {
    ShaderProgram* program = new ShaderProgram();
    program->addShader(*shaders[vertName]);
    program->addShader(*shaders[fragName]);
    program->link();
    shaderPrograms[name] = program;
}

void ResourceManager::loadTexture(const std::string& name, const std::string& path) {
    Texture* texture = new Texture();
    if (texture->loadFromFile(path)) {
        textures[name] = texture;
    } else {
        delete texture;
    }
}

Modell* ResourceManager::getModel(const std::string& name) {
    if (models.find(name) != models.end()) {
        return models[name];
    }
    return nullptr;
}

ShaderProgram* ResourceManager::getShaderProgram(const std::string& name) {
    if (shaderPrograms.find(name) != shaderPrograms.end()) {
        return shaderPrograms[name];
    }
    return nullptr;
}

Texture* ResourceManager::getTexture(const std::string& name) {
    if (textures.find(name) != textures.end()) {
        return textures[name];
    }
    return nullptr;
}
#include "ResourceManager.h"
#include <iostream>

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() {
    // Cleanup všech zdrojů
    for (auto& pair : models) delete pair.second;
    for (auto& pair : shaderPrograms) delete pair.second;
    for (auto& pair : shaders) delete pair.second;
    for (auto& pair : textures) delete pair.second;
}

void ResourceManager::loadModel(const std::string& name, const float* data, int dataSize, int stride) {
    Modell* model = new Modell();
    model->loadData(data, dataSize, stride);
    models[name] = model;
    std::cout << "✓ Loaded model: " << name << std::endl;
}

void ResourceManager::loadModelOBJ(const std::string& name, const std::string& path) {
    Modell* model = new Modell();
    if (model->loadOBJ(path)) {
        models[name] = model;
        std::cout << "✓ Loaded OBJ model: " << name << std::endl;
    } else {
        delete model;
        std::cerr << "✗ Failed to load OBJ: " << name << std::endl;
    }
}

void ResourceManager::loadModelWithTexCoords(const std::string& name, const float* data, int dataSize, int stride) {
    Modell* model = new Modell();
    model->loadDataWithTexCoords(data, dataSize, stride);
    models[name] = model;
    std::cout << "✓ Loaded textured model: " << name << std::endl;
}

void ResourceManager::loadShader(const std::string& name, GLenum type, const std::string& path) {
    Shader* shader = new Shader();
    shader->createShaderFromFile(type, path.c_str());
    shaders[name] = shader;
    std::cout << "✓ Loaded shader: " << name << std::endl;
}

void ResourceManager::loadShaderProgram(const std::string& name, 
                                       const std::string& vertName, 
                                       const std::string& fragName) {
    ShaderProgram* program = new ShaderProgram();
    program->addShader(*shaders[vertName]);
    program->addShader(*shaders[fragName]);
    program->link();
    shaderPrograms[name] = program;
    std::cout << "✓ Created shader program: " << name << std::endl;
}

void ResourceManager::loadTexture(const std::string& name, const std::string& path) {
    Texture* texture = new Texture();
    if (texture->loadFromFile(path)) {
        textures[name] = texture;
        std::cout << "✓ Loaded texture: " << name << std::endl;
    } else {
        delete texture;
        std::cerr << "✗ Failed to load texture: " << name << std::endl;
    }
}

Modell* ResourceManager::getModel(const std::string& name) {
    if (models.find(name) != models.end()) {
        return models[name];
    }
    std::cerr << "✗ Model not found: " << name << std::endl;
    return nullptr;
}

ShaderProgram* ResourceManager::getShaderProgram(const std::string& name) {
    if (shaderPrograms.find(name) != shaderPrograms.end()) {
        return shaderPrograms[name];
    }
    std::cerr << "✗ Shader program not found: " << name << std::endl;
    return nullptr;
}

Texture* ResourceManager::getTexture(const std::string& name) {
    if (textures.find(name) != textures.end()) {
        return textures[name];
    }
    std::cerr << "✗ Texture not found: " << name << std::endl;
    return nullptr;
}
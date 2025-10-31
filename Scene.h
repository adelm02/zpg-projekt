// Scene.h
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "DrawableObject.h"
#include "Light.h"
#include "ShaderProgram.h"

class Scene {
public:
    void addObject(DrawableObject* obj);
    void drawAll();
    void update(float dt);


    void addLight(const Light& l) { lights.push_back(l); }
    void clearLights() { lights.clear(); }
    void registerLightingShader(ShaderProgram* sp) { lightingShaders.push_back(sp); }

private:
    std::vector<DrawableObject*> objects;

    // --- NOVÉ ---
    std::vector<Light>            lights;
    std::vector<ShaderProgram*>   lightingShaders;

    void applyLightsTo(ShaderProgram* sp);
    void setGlobalUniforms(ShaderProgram* sp);
};

#endif // SCENE_H
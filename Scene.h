#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "DrawableObject.h"
#include "Light.h"
#include "ShaderProgram.h"
#include "SkyBox.h"

class Scene {
public:
    void addObject(DrawableObject* obj);
    void drawAll();
    void update(float dt);

    void addLight(const Light& l);
    void clearLights();
    void registerLightingShader(ShaderProgram* sp);
    void updateLight(int index, const Light& l);

    void setSkyBox(SkyBox* sb) { skybox = sb; }

    SkyBox* skybox = nullptr;

private:
    std::vector<DrawableObject*> objects;
    std::vector<Light> lights;
    std::vector<ShaderProgram*> lightingShaders;

    void applyLightsTo(ShaderProgram* sp);
    void setGlobalUniforms(ShaderProgram* sp);
};

#endif // SCENE_H
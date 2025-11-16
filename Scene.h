#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "DrawableObject.h"
#include "Light.h"
#include "ShaderProgram.h"
#include "Skybox.h"

class Scene {
public:
    void addObject(DrawableObject* obj);
    void removeObject(DrawableObject* obj);
    void drawAll();
    void drawAllWithStencil();
    void update(float dt);

    void addLight(const Light& l);
    void clearLights();
    void registerLightingShader(ShaderProgram* sp);
    void updateLight(int index, const Light& l);

    void setSkyBox(SkyBox* sb) { skybox = sb; }

    //object for poicking
    const std::vector<DrawableObject*>& getObjects() const { return objects; }

    SkyBox* skybox = nullptr;

private:
    std::vector<DrawableObject*> objects;
    std::vector<Light> lights;
    std::vector<ShaderProgram*> lightingShaders;

    void applyLightsTo(ShaderProgram* sp);
    void setGlobalUniforms(ShaderProgram* sp);
};

#endif // SCENE_H
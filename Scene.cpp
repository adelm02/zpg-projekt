// Scene.cpp
#include "Scene.h"
#include "Camera.h"
#include <algorithm>

void Scene::addObject(DrawableObject* obj) {
    objects.push_back(obj);
}

void Scene::setGlobalUniforms(ShaderProgram* sp) {
    sp->SetUniform("viewMatrix",Camera::getInstance()->getCamera());
    sp->SetUniform("projectionMatrix",Camera::getInstance()->getProjection());
    sp->SetUniform("viewPos",Camera::getInstance()->getCameraPos());
}

void Scene::applyLightsTo(ShaderProgram* sp) {
    const int nol = std::min((int)lights.size(), 30);
    sp->SetUniform("nol", nol);
    sp->SetUniform("lightCount", nol);
    for (int i = 0; i < nol; ++i) {
        const Light& L = lights[i];
        std::string base = "lights[" + std::to_string(i) + "]";

        sp->SetUniform((base + ".type").c_str(), L.type);
        sp->SetUniform((base + ".position").c_str(), L.position);
        sp->SetUniform((base + ".color").c_str(), L.color);
        sp->SetUniform((base + ".atten").c_str(), L.atten);
        sp->SetUniform((base + ".direction").c_str(), L.direction);
        sp->SetUniform((base + ".cutoff").c_str(), L.cutoff);
    }
}

void Scene::drawAll() {
    for (auto* sp : lightingShaders) {
        if (!sp) continue;
        sp->useShaderProgram();
        setGlobalUniforms(sp);
        applyLightsTo(sp);
    }

    for (auto* obj : objects) {
        if (obj) obj->draw();
    }
}

void Scene::update(float dt) {
    for (auto* obj : objects) {
        if (obj) obj->update(dt);
    }
}

void Scene::updateLight(int index, const Light &l) {
    if (index >= 0 && index < (int)lights.size()) {
        lights[index] = l;
    }
}
// Scene.cpp
// Upraveno pro podporu stencil bufferu
#include "Scene.h"
#include "Camera.h"
#include <algorithm>
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
    if (skybox) {
        skybox->draw(
            Camera::getInstance()->getCamera(),
            Camera::getInstance()->getProjection()
        );
    }
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

void Scene::drawAllWithStencil() {
    // Povolit stencil test
    glEnable(GL_STENCIL_TEST);

    // Vyčistit stencil buffer na 0
    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);

    // Nakreslit skybox (bez stencil zápisu)
    if (skybox) {
        glStencilMask(0x00); // Zakázat zápis do stencil bufferu
        skybox->draw(
            Camera::getInstance()->getCamera(),
            Camera::getInstance()->getProjection()
        );
    }

    // Povolit zápis do stencil bufferu pro objekty
    glStencilMask(0xFF);

    // Nastavit uniformy pro všechny shadery
    for (auto* sp : lightingShaders) {
        if (!sp) continue;
        sp->useShaderProgram();
        setGlobalUniforms(sp);
        applyLightsTo(sp);
    }

    // Vykreslit každý objekt s jeho stencil ID
    int stencilIndex = 1; // Začínáme od 1, 0 je pro pozadí
    for (auto* obj : objects) {
        if (obj) {
            obj->setStencilID(stencilIndex);
            obj->drawWithStencil(stencilIndex);
            stencilIndex++;

            // Omezení na max 255 objektů (limit stencil bufferu)
            if (stencilIndex > 255) {
                stencilIndex = 1;
            }
        }
    }

    // Vypnout stencil test po vykreslení
    glDisable(GL_STENCIL_TEST);
}

void Scene::update(float dt) {
    for (auto* obj : objects) {
        if (obj) obj->update(dt);
    }
}

void Scene::addLight(const Light &l) {
    lights.push_back(l);
}

void Scene::clearLights() {
    lights.clear();
}

void Scene::registerLightingShader(ShaderProgram *sp) {
    lightingShaders.push_back(sp);
}

void Scene::updateLight(int index, const Light &l) {
    if (index >= 0 && index < (int)lights.size()) {
        lights[index] = l;
    }
}

void Scene::removeObject(DrawableObject* obj) {
    auto it = std::find(objects.begin(), objects.end(), obj);
    if (it != objects.end()) {
        objects.erase(it);
    }
}
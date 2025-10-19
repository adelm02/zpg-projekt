
#include "SceneManager.h"

#include <iostream>
#include <ostream>

void SceneManager::addScene(Scene* scene) {
    scenes.push_back(scene);
}

void SceneManager::switchScene(int index) {
    if (index >= 0 && index < scenes.size())
        currentSceneIndex = index;
}

void SceneManager::drawCurrentScene() {
    if (scenes.empty() || currentSceneIndex < 0 || currentSceneIndex >= (int)scenes.size())
        return;
    if (scenes[currentSceneIndex] == nullptr)
        return;

    scenes[currentSceneIndex]->drawAll();
}

void SceneManager::update(float dt) {
    if (scenes.empty() || currentSceneIndex < 0 || currentSceneIndex >= (int)scenes.size())
        return;
    if (scenes[currentSceneIndex] == nullptr)
        return;
    scenes[currentSceneIndex]->update(dt);
}

int SceneManager::getCurrentSceneIndex() {
    return currentSceneIndex;
}

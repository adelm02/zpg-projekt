#include "Scene.h"

#include <iostream>
#include <ostream>

void Scene::addObject(DrawableObject* obj) {
    objects.push_back(obj);
}

void Scene::drawAll() {
    for (auto* obj : objects) {
        if (obj) obj->draw();
    }
}

void Scene::update(float dt) {
    for (auto* obj : objects) {
        if (obj) obj->update(dt);
    }
}

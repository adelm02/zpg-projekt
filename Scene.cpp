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
    std::cout << dt << std::endl;
    for (auto* obj : objects) {
        if (obj) obj->update(dt);
    }
}

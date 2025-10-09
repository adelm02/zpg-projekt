#ifndef ZPGPROJ_SCENE_H
#define ZPGPROJ_SCENE_H

#include <vector>
#include "DrawableObject.h"

class Scene {
public:
    std::vector<DrawableObject*> objects;

    ~Scene() = default;

    void addObject(DrawableObject* obj);
    void drawAll();
    void update(float dt);
};

#endif // ZPGPROJ_SCENE_H
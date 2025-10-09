//
// Created by Adéla Martynková on 05.10.2025.
//

#ifndef ZPGPROJ_SCENEMANAGER_H
#define ZPGPROJ_SCENEMANAGER_H
#include <vector>

#include "scene.h"


class SceneManager {
private:
    std::vector<Scene*> scenes;
    int currentSceneIndex = 0;

public:
    void addScene(Scene* scene);
    void switchScene(int index);
    void drawCurrentScene();
    void update(float dt);
};



#endif //ZPGPROJ_SCENEMANAGER_H

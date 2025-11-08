#ifndef ZPGPROJ_SCENEMANAGER_H
#define ZPGPROJ_SCENEMANAGER_H

#include <vector>
#include "Scene.h"
#include "ResourceManager.h"
#include "Firefly.h"
#include "Rotate.h"

class SceneManager {
private:
    std::vector<Scene*> scenes;
    int currentSceneIndex = 0;
    
    ResourceManager resourceManager;
    

    std::vector<DrawableObject*> drawableObjects;
    std::vector<Transformation*> transformations;
    std::vector<Scale*> scales;
    std::vector<Tranform*> tranforms;
    std::vector<Rotate*> rotations;
    std::vector<Firefly*> fireflies;
    std::vector<Scene*> ownedScenes;

public:
    SceneManager();
    ~SceneManager();

    void addScene(Scene* scene);
    void switchScene(int index);
    void drawCurrentScene();
    void update(float dt);
    int getCurrentSceneIndex();
    Scene* getCurrentScene();
    
    void initializeScenes();

private:
    void loadAllResources();
    void createScene1();
    void createScene2();
    void createScene3();
    void createScene4();
};

#endif
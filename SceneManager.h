#ifndef ZPGPROJ_SCENEMANAGER_H
#define ZPGPROJ_SCENEMANAGER_H

#include <vector>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "ResourceManager.h"
#include "Firefly.h"
#include "Rotate.h"
#include "ObjectManager.h"

class SceneManager {
private:
    std::vector<Scene*> scenes;
    int currentSceneIndex = 0;
    int selectedObjectIndex = -1; // Index vybraného objektu (-1 = žádný)

    ResourceManager resourceManager;
    ObjectManager* objectManager;

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

    void setSelectedObject(int index);
    int getSelectedObject() const { return selectedObjectIndex; }
    DrawableObject* getObjectByIndex(int index);
    ObjectManager* getObjectManager() { return objectManager; }
    void registerSceneObjectsToManager();



private:
    void loadAllResources();
    void createScene1();
    void createScene2();
    void createScene3();
    void createScene4();
};

#endif
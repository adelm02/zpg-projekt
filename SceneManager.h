//
// Created by Adéla Martynková on 05.10.2025.
//

#ifndef ZPGPROJ_SCENEMANAGER_H
#define ZPGPROJ_SCENEMANAGER_H
#include <vector>

#include "Scene.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Modell.h"
#include "Transformation.h"
#include "Scale.h"
#include "Tranform.h"
#include "DrawableObject.h"


class SceneManager {
private:
    std::vector<Scene*> scenes;
    int currentSceneIndex = 0;

    // Persistent objects that need to outlive initializeScenes()
    std::vector<Shader*> shaders;
    std::vector<ShaderProgram*> shaderPrograms;
    std::vector<Modell*> models;
    std::vector<Scene*> ownedScenes;
    std::vector<Transformation*> transformations;
    std::vector<DrawableObject*> drawableObjects;
    std::vector<Scale*> scales;
    std::vector<Tranform*> tranforms;

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
};



#endif //ZPGPROJ_SCENEMANAGER_H


#include "SceneManager.h"

#include <iostream>
#include <ostream>
#include <cstdlib>
#include <ctime>
#include "Shader.h"
#include "ShaderProgram.h"
#include "Modell.h"
#include "Transformation.h"
#include "DrawableObject.h"
#include "Rotate.h"
#include "Scale.h"
#include "Tranform.h"
#include "Light.h"
#include "tree.h"
#include "sphere.h"
#include "bushes.h"
#include "suzi_smooth.h"
#include "triangle.h"
#include "plain.h"
#include <glm/glm.hpp>

extern Tranform moveEarth;
extern Tranform moveMoon;
extern Transformation tEarth;
extern Transformation tMoon;

SceneManager::SceneManager() {}

SceneManager::~SceneManager() {
    // Clean up all allocated objects
    for (auto* obj : drawableObjects) delete obj;
    for (auto* obj : transformations) delete obj;
    for (auto* obj : scales) delete obj;
    for (auto* obj : tranforms) delete obj;
    for (auto* obj : ownedScenes) delete obj;
    for (auto* obj : models) delete obj;
    for (auto* obj : shaderPrograms) delete obj;
    for (auto* obj : shaders) delete obj;
    for (auto* obj : myTextures) delete obj;
}

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

Scene* SceneManager::getCurrentScene() {
    if (currentSceneIndex >= 0 && currentSceneIndex < (int)scenes.size()) {
        return scenes[currentSceneIndex];
    }
    return nullptr;
}

void SceneManager::initializeScenes() {
    std::srand(42);


    Shader* vertex = new Shader();
    vertex->createShaderFromFile(GL_VERTEX_SHADER, "../shaders/vertex.vert");

    Shader* fragment_lambert = new Shader();
    fragment_lambert->createShaderFromFile(GL_FRAGMENT_SHADER, "../shaders/lambert.frag");

    Shader* fragment_phong = new Shader();
    fragment_phong->createShaderFromFile(GL_FRAGMENT_SHADER, "../shaders/phong.frag");

    Shader* fragment_blinn = new Shader();
    fragment_blinn->createShaderFromFile(GL_FRAGMENT_SHADER, "../shaders/blinn.frag");

    Shader* fragC = new Shader();
    fragC->createShaderFromFile(GL_FRAGMENT_SHADER, "../shaders/fragmentColor.frag");

    Shader* fragment_phong_light = new Shader();
    fragment_phong_light->createShaderFromFile(GL_FRAGMENT_SHADER, "../shaders/fragment_phong_light.frag");


    ShaderProgram* programLambert = new ShaderProgram();
    programLambert->addShader(*vertex);
    programLambert->addShader(*fragment_lambert);
    programLambert->link();

    ShaderProgram* programPhong = new ShaderProgram();
    programPhong->addShader(*vertex);
    programPhong->addShader(*fragment_phong);
    programPhong->link();

    ShaderProgram* programPhongLight = new ShaderProgram();
    programPhongLight->addShader(*vertex);
    programPhongLight->addShader(*fragment_phong_light);
    programPhongLight->link();

    ShaderProgram* programBlinn = new ShaderProgram();
    programBlinn->addShader(*vertex);
    programBlinn->addShader(*fragment_blinn);
    programBlinn->link();

    ShaderProgram* programConstant = new ShaderProgram();
    programConstant->addShader(*vertex);
    programConstant->addShader(*fragC);
    programConstant->link();

    Modell* strom = new Modell();
    strom->loadData(tree, sizeof(tree)/sizeof(float), 6);
    Modell* bush = new Modell();
    bush->loadData(bushes, sizeof(bushes)/sizeof(float), 6);
    Modell* koule = new Modell();
    koule->loadData(sphere, sizeof(sphere)/sizeof(float), 6);
    Modell* tria = new Modell();
    tria->loadData(triangle, sizeof(triangle)/sizeof(float), 6);
    Modell* pl = new Modell();
    pl->loadData(plain, sizeof(plain)/sizeof(float), 6);
    Modell* formula = new Modell();
    formula->loadOBJ("assets/formula1.obj");

    Texture* grassTexture = new Texture();
    if (!grassTexture->loadFromFile("assets/grass.png")) {
        std::cerr << "Chyba: Nelze nacist texturu" << std::endl;
        delete grassTexture;
        grassTexture = nullptr;
    }


    Scene* scene1 = new Scene();
    Scene* scene2 = new Scene();
    Scene* scene3 = new Scene();
    Scene* scene4 = new Scene();

    scene3->registerLightingShader(programPhongLight);
    scene4->registerLightingShader(programPhongLight);
    scene1->registerLightingShader(programBlinn);


    scene1->addLight( Light(0, glm::vec3(-0.3f, -1.0f, -0.2f), glm::vec3(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) );

    scene1->addLight( Light(glm::vec3(0.0f, 0.0f, 0.0f), //pos
                            glm::vec3(0.0f, 0.0f, -1.0f), //dir
                            glm::vec3(1.0f, 1.0f, 0.9f),
                            glm::radians(25.0f),
                            glm::vec3(1.0f, 0.09f, 0.032f))); //atten


    scene4->addLight( Light(glm::vec3(5.0f, 6.0f, 5.0f),
                           glm::vec3(1.0f, 0.95f, 0.85f),
                           glm::vec3(1.0f, 0.14f, 0.07f)) );

    // Setup lights for scene3 (fireflies)
    const int N = 6;
    std::srand(42);
    for (int i = 0; i < N; ++i) {
        float rx = (std::rand() / (float)RAND_MAX) * 90.0f;
        float rz = (std::rand() / (float)RAND_MAX) * 40.0f;
        float ry = 2.2f + (std::rand() / (float)RAND_MAX) * 1.4f;

    Light L(
        glm::vec3(rx, ry, rz),
        glm::vec3(1.00f, 1.0f, 1.0f),
        glm::vec3(1.0f, 0.22f, 0.20f)
    );

        scene3->addLight(L);
    }
    

    scene3->addLight( Light(glm::vec3(0.0f, 0.0f, 0.0f), //pos
                            glm::vec3(0.0f, 0.0f, -1.0f), //dir
                            glm::vec3(1.0f, 1.0f, 1.0f),
                            glm::radians(25.0f),
                            glm::vec3(1.0f, 0.09f, 0.032f))); //atten

    // Create firefly objects
    std::srand(42);
    for (int i = 0; i < N; ++i) {
        float rx = (std::rand() / (float)RAND_MAX) * 90.0f;
        float rz = (std::rand() / (float)RAND_MAX) * 40.0f;
        float ry = 2.2f + (std::rand() / (float)RAND_MAX) * 1.4f;

        Scale* sFly = new Scale(0.12f, 0.12f, 0.12f);
        Tranform* mFly = new Tranform(rx, ry, rz);
        Transformation* tf = new Transformation();

        tf->addTrans(sFly);
        tf->addTrans(mFly);

        DrawableObject* fly = new DrawableObject(*koule, *programConstant, *tf, glm::vec3(1.0f,1.0f,1.0f));
        scene3->addObject(fly);

        scales.push_back(sFly);
        tranforms.push_back(mFly);
        transformations.push_back(tf);
        drawableObjects.push_back(fly);
    }

    // Create trees and bushes
    for (int i = 0; i < 50; i++) {
        float x = (i % 10) * 10.0f;
        float z = (i / 10) * 10.0f;
        float x1 = x + 5.0f;
        float z1 = z + 5.0f;

        // Tree
        Tranform* move = new Tranform(x, 0.0f, z);
        Transformation* t = new Transformation();
        t->addTrans(move);

        DrawableObject* stromek = new DrawableObject(*strom, *programPhongLight, *t, glm::vec3(0.20f, 0.65f, 0.40f));
        scene3->addObject(stromek);

        tranforms.push_back(move);
        transformations.push_back(t);
        drawableObjects.push_back(stromek);

        // Bush
        Scale* s = new Scale(3.f, 3.f, 3.f);
        Tranform* moveBush = new Tranform(x1, 0.0f, z1);
        Transformation* tBush = new Transformation();
        tBush->addTrans(s);
        tBush->addTrans(moveBush);

        DrawableObject* bushi = new DrawableObject(*bush, *programPhongLight, *tBush, glm::vec3(0.12f, 0.45f, 0.18f));
        scene3->addObject(bushi);

        scales.push_back(s);
        tranforms.push_back(moveBush);
        transformations.push_back(tBush);
        drawableObjects.push_back(bushi);
    }

    // Create ground plane
    Scale* zem = new Scale(50.0f, 1.0f, 30.0f);
    Scale* mini = new Scale(.5f, .5f, .5f);
    Tranform* groundMove = new Tranform(45.0f, 0.0f, 20.0f);
    Transformation* zemm = new Transformation();
    Transformation* minii = new Transformation();
    zemm->addTrans(zem);
    zemm->addTrans(groundMove);
    minii->addTrans(mini);

    DrawableObject* plain = new DrawableObject(*pl, *programPhongLight, *zemm, glm::vec3(0.27f, 0.25f, 0.15f));
    plain->setTexture(grassTexture);
    scene3->addObject(plain);

    scales.push_back(zem);
    tranforms.push_back(groundMove);
    transformations.push_back(zemm);
    drawableObjects.push_back(plain);
    scales.push_back(mini);
    transformations.push_back(minii);


    Tranform* stred = new Tranform(0.f, 0.f, 0.f);
    Transformation* middle = new Transformation();
    middle->addTrans(stred);


    Scale* earthScale = new Scale(0.6f, 0.6f, 0.6f);
    Scale* moonScale = new Scale(0.25f, 0.25f, 0.25f);
    tEarth.addTrans(earthScale);
    tEarth.addTrans(&moveEarth);
    tMoon.addTrans(moonScale);
    tMoon.addTrans(&moveMoon);


    DrawableObject* formula1 = new DrawableObject(*formula, *programBlinn, *minii);
    formula1->setMaterial(formula->getMaterial());
    scene1->addObject(formula1);

    DrawableObject* earthObject = new DrawableObject(*koule, *programPhongLight, tEarth);
    DrawableObject* moonObject = new DrawableObject(*koule, *programPhongLight, tMoon);
    DrawableObject* idk = new DrawableObject(*koule, *programPhong, tEarth);
    scene4->addObject(earthObject);
    scene4->addObject(moonObject);
    scene4->addObject(idk);

    scales.push_back(earthScale);
    scales.push_back(moonScale);
    drawableObjects.push_back(earthObject);
    drawableObjects.push_back(moonObject);
    drawableObjects.push_back(idk);


    Scale* zem2 = new Scale(50.0f, 1.0f, 30.0f);
    Tranform* groundMove2 = new Tranform(45.0f, 0.0f, 20.0f);
    Transformation* zemm2 = new Transformation();
    zemm2->addTrans(zem2);
    zemm2->addTrans(groundMove2);

    DrawableObject* triangleObj = new DrawableObject(*koule, *programBlinn, *middle);
    scene1->addObject(triangleObj);
    scene1->addObject(formula1);

    tranforms.push_back(stred);
    transformations.push_back(middle);
    scales.push_back(zem2);
    tranforms.push_back(groundMove2);
    transformations.push_back(zemm2);
    drawableObjects.push_back(triangleObj);
    drawableObjects.push_back(formula1);

    // Store all the objects before adding scenes
    shaders.push_back(vertex);
    shaders.push_back(fragment_lambert);
    shaders.push_back(fragment_phong);
    shaders.push_back(fragment_blinn);
    shaders.push_back(fragC);
    shaders.push_back(fragment_phong_light);

    shaderPrograms.push_back(programLambert);
    shaderPrograms.push_back(programPhong);
    shaderPrograms.push_back(programPhongLight);
    shaderPrograms.push_back(programBlinn);
    shaderPrograms.push_back(programConstant);

    models.push_back(strom);
    models.push_back(bush);
    models.push_back(koule);
    models.push_back(tria);
    models.push_back(pl);
    models.push_back(formula);


    myTextures.push_back(grassTexture);


    addScene(scene1);
    addScene(scene2);
    addScene(scene3);
    addScene(scene4);

    ownedScenes.push_back(scene1);
    ownedScenes.push_back(scene2);
    ownedScenes.push_back(scene3);
    ownedScenes.push_back(scene4);
}

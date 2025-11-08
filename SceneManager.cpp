#include "SceneManager.h"
#include "ObjectFactory.h"
#include "Skybox.h"
#include <cstdlib>
#include <iostream>
#include "bushes.h"
#include "tree.h"
#include "sphere.h"
#include "triangle.h"
#include "plain_textured.h"


extern Tranform moveEarth;
extern Tranform moveMoon;
extern Transformation tEarth;
extern Transformation tMoon;

SceneManager::SceneManager() {}

SceneManager::~SceneManager() {
    for (auto* obj : drawableObjects) delete obj;
    for (auto* obj : transformations) delete obj;
    for (auto* obj : scales) delete obj;
    for (auto* obj : tranforms) delete obj;
    for (auto* obj : rotations) delete obj;
    for (auto* obj : fireflies) delete obj;
    for (auto* obj : ownedScenes) delete obj;
}

void SceneManager::initializeScenes() {
    std::srand(42);

    loadAllResources();

    //createScene1();
    //createScene2();
    createScene3();
    //createScene4();
}

void SceneManager::loadAllResources() {
    std::cout << "\n=== Loading Resources ===" << std::endl;

    // SHADERY
    resourceManager.loadShader("vertex", GL_VERTEX_SHADER, "../shaders/vertex.vert");
    resourceManager.loadShader("fragment_lambert", GL_FRAGMENT_SHADER, "../shaders/lambert.frag");
    resourceManager.loadShader("fragment_phong", GL_FRAGMENT_SHADER, "../shaders/phong.frag");
    resourceManager.loadShader("fragment_blinn", GL_FRAGMENT_SHADER, "../shaders/blinn.frag");
    resourceManager.loadShader("fragment_constant", GL_FRAGMENT_SHADER, "../shaders/fragmentColor.frag");
    resourceManager.loadShader("fragment_phong_light", GL_FRAGMENT_SHADER, "../shaders/fragment_phong_light.frag");
    resourceManager.loadShader("vertex_skybox", GL_VERTEX_SHADER, "../shaders/skybox.vert");
    resourceManager.loadShader("fragment_skybox", GL_FRAGMENT_SHADER, "../shaders/skybox.frag");

    // SHADER PROGRAMY
    resourceManager.loadShaderProgram("lambert", "vertex", "fragment_lambert");
    resourceManager.loadShaderProgram("phong", "vertex", "fragment_phong");
    resourceManager.loadShaderProgram("phong_light", "vertex", "fragment_phong_light");
    resourceManager.loadShaderProgram("blinn", "vertex", "fragment_blinn");
    resourceManager.loadShaderProgram("constant", "vertex", "fragment_constant");
    resourceManager.loadShaderProgram("skybox", "vertex_skybox", "fragment_skybox");

    // MODELY
    resourceManager.loadModel("tree", tree, sizeof(tree)/sizeof(float), 6);
    resourceManager.loadModel("bush", bushes, sizeof(bushes)/sizeof(float), 6);
    resourceManager.loadModel("sphere", sphere, sizeof(sphere)/sizeof(float), 6);
    resourceManager.loadModel("triangle", triangle, sizeof(triangle)/sizeof(float), 6);
    resourceManager.loadModelWithTexCoords("plane", plain_textured, sizeof(plain_textured)/sizeof(float), 8);

    resourceManager.loadModelOBJ("formula", "assets/formula1.obj");
    resourceManager.loadModelOBJ("shrek", "assets/shrek/shrek.obj");
    resourceManager.loadModelOBJ("fiona", "assets/shrek/fiona.obj");
    resourceManager.loadModelOBJ("toilet", "assets/shrek/toiled.obj");

    // TEXTURY
    resourceManager.loadTexture("grass", "assets/grass.jpg");
    resourceManager.loadTexture("shrek", "assets/shrek/shrek.png");
    resourceManager.loadTexture("fiona", "assets/shrek/fiona.png");
    resourceManager.loadTexture("toilet", "assets/shrek/toiled.jpg");

    std::cout << "=== Resources Loaded ===" << std::endl;
}



void SceneManager::createScene3() {
    std::cout << "\n=== Creating Scene 3 (Forest) ===" << std::endl;

    Scene* scene3 = new Scene();
    scene3->registerLightingShader(resourceManager.getShaderProgram("phong_light"));


    for (int i = 0; i < 8; ++i) {
        float rx = (std::rand() / (float)RAND_MAX) * 80.0f;
        float rz = (std::rand() / (float)RAND_MAX) * 48.0f;
        float ry = 2.2f + (std::rand() / (float)RAND_MAX) * 1.9f;

        Firefly* firefly = new Firefly(
            glm::vec3(rx, ry, rz),
            *resourceManager.getModel("sphere"),
            *resourceManager.getShaderProgram("constant")
        );

        scene3->addLight(firefly->getLight());
        scene3->addObject(firefly->getVisualObject());

        fireflies.push_back(firefly);
        drawableObjects.push_back(firefly->getVisualObject());
        scales.push_back(firefly->getScale());
        tranforms.push_back(firefly->getTransform());
        transformations.push_back(firefly->getTransformation());
    }

    // FLASHLIGHT
    scene3->addLight(Light(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::radians(25.0f),
        glm::vec3(1.0f, 0.09f, 0.032f)
    ));


    for (int i = 0; i < 50; i++) {
        float x = (i % 10) * 10.0f;
        float z = (i / 10) * 10.0f;

        auto tree = ObjectFactory::createTree(
            glm::vec3(x, 0.0f, z),
            *resourceManager.getModel("tree"),
            *resourceManager.getShaderProgram("phong_light")
        );

        scene3->addObject(tree.object);
        drawableObjects.push_back(tree.object);
        for (auto* t : tree.transforms) tranforms.push_back(t);
        transformations.push_back(tree.transformation);
    }


    for (int i = 0; i < 50; i++) {
        float x = (i % 10) * 10.0f + 5.0f;
        float z = (i / 10) * 10.0f + 5.0f;

        auto bush = ObjectFactory::createBush(
            glm::vec3(x, 0.0f, z),
            *resourceManager.getModel("bush"),
            *resourceManager.getShaderProgram("phong_light")
        );

        scene3->addObject(bush.object);
        drawableObjects.push_back(bush.object);
        for (auto* s : bush.scales) scales.push_back(s);
        for (auto* t : bush.transforms) tranforms.push_back(t);
        transformations.push_back(bush.transformation);
    }


    auto ground = ObjectFactory::createGroundPlane(
        glm::vec3(45.0f, 0.0f, 20.0f),
        glm::vec3(50.0f, 1.0f, 30.0f),
        *resourceManager.getModel("plane"),
        *resourceManager.getShaderProgram("phong_light")
    );
    ground.object->setTexture(resourceManager.getTexture("grass"));
    scene3->addObject(ground.object);



    drawableObjects.push_back(ground.object);
    for (auto* s : ground.scales) scales.push_back(s);
    for (auto* t : ground.transforms) tranforms.push_back(t);
    transformations.push_back(ground.transformation);

    // Shrek
    auto shrek = ObjectFactory::createCharacter(
        glm::vec3(42.0f, 0.0f, 22.0f),
        90.0f,
        2.0f,
        *resourceManager.getModel("shrek"),
        *resourceManager.getShaderProgram("constant")
    );
    shrek.object->setTexture(resourceManager.getTexture("shrek"));
    scene3->addObject(shrek.object);

    drawableObjects.push_back(shrek.object);
    for (auto* s : shrek.scales) scales.push_back(s);
    for (auto* r : shrek.rotations) rotations.push_back(r);
    for (auto* t : shrek.transforms) tranforms.push_back(t);
    transformations.push_back(shrek.transformation);

    // Fiona
    auto fiona = ObjectFactory::createCharacter(
        glm::vec3(48.0f, 0.0f, 22.0f),
        -90.0f,
        2.0f,
        *resourceManager.getModel("fiona"),
        *resourceManager.getShaderProgram("constant")
    );
    fiona.object->setTexture(resourceManager.getTexture("fiona"));
    scene3->addObject(fiona.object);

    drawableObjects.push_back(fiona.object);
    for (auto* s : fiona.scales) scales.push_back(s);
    for (auto* r : fiona.rotations) rotations.push_back(r);
    for (auto* t : fiona.transforms) tranforms.push_back(t);
    transformations.push_back(fiona.transformation);

    // Toilet
    auto toilet = ObjectFactory::createCharacter(
        glm::vec3(45.0f, 0.0f, 17.0f),
        0.0f,
        1.5f,
        *resourceManager.getModel("toilet"),
        *resourceManager.getShaderProgram("phong_light")
    );
    toilet.object->setTexture(resourceManager.getTexture("toilet"));
    scene3->addObject(toilet.object);

    drawableObjects.push_back(toilet.object);
    for (auto* s : toilet.scales) scales.push_back(s);
    for (auto* r : toilet.rotations) rotations.push_back(r);
    for (auto* t : toilet.transforms) tranforms.push_back(t);
    transformations.push_back(toilet.transformation);

    addScene(scene3);
    ownedScenes.push_back(scene3);
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
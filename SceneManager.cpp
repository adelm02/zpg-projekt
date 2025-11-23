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
#include "CustomTrans.h"
#include "ArcadeGame.h"
#include "PathMovement.h"
#include "GameTarget.h"
#include "OrbitTransform.h"

SceneManager::SceneManager() : arcadeGame(nullptr), earthOrbit(nullptr), moonOrbit(nullptr) {
    objectManager = new ObjectManager();
}

SceneManager::~SceneManager() {
    for (auto* obj : drawableObjects) delete obj;
    for (auto* obj : transformations) delete obj;
    for (auto* obj : scales) delete obj;
    for (auto* obj : tranforms) delete obj;
    for (auto* obj : rotations) delete obj;
    for (auto* obj : fireflies) delete obj;
    for (auto* obj : ownedScenes) delete obj;
    if (arcadeGame) delete arcadeGame;
    if (earthOrbit) delete earthOrbit;
    if (moonOrbit) delete moonOrbit;
}

void SceneManager::initializeScenes() {
    std::srand(std::time(nullptr));
    loadAllResources();

    createScene1();
    createScene2();
    createScene3();
    createScene4();

    if (objectManager && !scenes.empty()) {
        objectManager->setScene(scenes[0]);
        registerSceneObjectsToManager();
    }
}

void SceneManager::registerSceneObjectsToManager() {
    if (!objectManager) {
        return;
    }

    Scene* currentScene = getCurrentScene();
    if (!currentScene) {
        std::cout << "Error: No current scene!" << std::endl;
        return;
    }

    objectManager->deselect();
    objectManager->clear();

    objectManager->setScene(currentScene);

    const auto& sceneObjects = currentScene->getObjects();

    for (size_t i = 0; i < sceneObjects.size(); i++) {
        DrawableObject* obj = sceneObjects[i];
        if (!obj) continue;

        ObjectData data;
        data.object = obj;
        data.originalColor = obj->color;
        data.stencilID = i + 1;

        auto it = objectToTransform.find(obj);
        if (it != objectToTransform.end()) {
            data.transform = it->second;
        } else {
            data.transform = nullptr;
        }

        data.transformation = nullptr;
        data.scale = nullptr;
        data.rotation = nullptr;

        objectManager->addObjectWithoutScene(data);
    }

    std::cout << "ObjectManager has "<< objectManager->getObjectCount() << " objects" << std::endl;}

void SceneManager::plantTreeAt(const glm::vec3 &worldPos) {
    Scene* scene = getCurrentScene();
    if (!scene) {
        std::cout << "No current scene for planting tree" << std::endl;
        return;
    }

    if (currentSceneIndex != 2) {
        std::cout << "Tree planting allowed only in scene 3" << std::endl;
        return;
    }

    glm::vec3 pos = glm::vec3(worldPos.x, 0.0f, worldPos.z);

    auto tree = ObjectFactory::createTree(
        pos,
        *resourceManager.getModel("tree"),
        *resourceManager.getShaderProgram("phong_light")
    );

    scene->addObject(tree.object);

    drawableObjects.push_back(tree.object);
    for (auto* t : tree.transforms) tranforms.push_back(t);
    transformations.push_back(tree.transformation);
    if (!tree.transforms.empty()) {
        registerObjectTransform(tree.object, tree.transforms[0]);
    }

    if (objectManager) {
        ObjectData data;
        data.object = tree.object;
        data.originalColor = tree.object->color;
        data.stencilID = 0;
        data.transformation = tree.transformation;
        data.scale = tree.scales.empty() ? nullptr : tree.scales[0];
        data.transform = tree.transforms.empty() ? nullptr : tree.transforms[0];
        data.rotation = tree.rotations.empty() ? nullptr : tree.rotations[0];

        objectManager->addObjectWithoutScene(data);
        objectManager->updateStencilIDs();
    }
    std::cout << "Tree planted at: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
}

void SceneManager::moveSelectedObject(const glm::vec3& offset) {
    if (!objectManager || !objectManager->hasSelection()) {
        return;
    }

    int selectedIndex = objectManager->getSelectedIndex();
    const auto& objects = objectManager->getObjects();

    if (selectedIndex < 0 || selectedIndex >= (int)objects.size()) {
        return;
    }

    const ObjectData& objData = objects[selectedIndex];
    Tranform* transform = objData.transform;

    if (!transform) {
        return;
    }

    transform->x += offset.x;
    transform->z += offset.z;

    std::cout << "Object at: ("<< transform->x << ", "<< transform->y << ", "<< transform->z << ")" << std::endl;}

void SceneManager::registerObjectTransform(DrawableObject* obj, Tranform* transform) {
    objectToTransform[obj] = transform;
}

void SceneManager::loadAllResources() {
    // Shaders
    resourceManager.loadShader("vertex", GL_VERTEX_SHADER, "../shaders/vertex.vert");
    resourceManager.loadShader("fragment_lambert", GL_FRAGMENT_SHADER, "../shaders/lambert.frag");
    resourceManager.loadShader("fragment_phong", GL_FRAGMENT_SHADER, "../shaders/phong.frag");
    resourceManager.loadShader("fragment_blinn", GL_FRAGMENT_SHADER, "../shaders/blinn.frag");
    resourceManager.loadShader("fragment_constant", GL_FRAGMENT_SHADER, "../shaders/fragmentColor.frag");
    resourceManager.loadShader("fragment_phong_light", GL_FRAGMENT_SHADER, "../shaders/fragment_phong_light.frag");
    resourceManager.loadShader("vertex_skybox", GL_VERTEX_SHADER, "../shaders/skybox.vert");
    resourceManager.loadShader("fragment_skybox", GL_FRAGMENT_SHADER, "../shaders/skybox.frag");

    // ShaderPrograms
    resourceManager.loadShaderProgram("lambert", "vertex", "fragment_lambert");
    resourceManager.loadShaderProgram("phong", "vertex", "fragment_phong");
    resourceManager.loadShaderProgram("phong_light", "vertex", "fragment_phong_light");
    resourceManager.loadShaderProgram("blinn", "vertex", "fragment_blinn");
    resourceManager.loadShaderProgram("constant", "vertex", "fragment_constant");
    resourceManager.loadShaderProgram("skybox", "vertex_skybox", "fragment_skybox");

    // Modells
    resourceManager.loadModel("tree", tree, sizeof(tree)/sizeof(float), 6);
    resourceManager.loadModel("bush", bushes, sizeof(bushes)/sizeof(float), 6);
    resourceManager.loadModel("sphere", sphere, sizeof(sphere)/sizeof(float), 6);
    resourceManager.loadModel("triangle", triangle, sizeof(triangle)/sizeof(float), 6);
    resourceManager.loadModelWithTexCoords("plane", plain_textured, sizeof(plain_textured)/sizeof(float), 8);


    resourceManager.loadModelOBJ("formula", "assets/formula1.obj");
    resourceManager.loadModelOBJ("shrek", "assets/shrek/shrek.obj");
    resourceManager.loadModelOBJ("fiona", "assets/shrek/fiona.obj");
    resourceManager.loadModelOBJ("toilet", "assets/shrek/toiled.obj");
    resourceManager.loadModelOBJ("koule", "assets/planet.obj");

    // Textures
    resourceManager.loadTexture("grass", "assets/grass.jpg");
    resourceManager.loadTexture("shrek", "assets/shrek/shrek.png");
    resourceManager.loadTexture("fiona", "assets/shrek/fiona.png");
    resourceManager.loadTexture("toilet", "assets/shrek/toiled.jpg");
    resourceManager.loadTexture("sun", "assets/planets/sun.jpg");
    resourceManager.loadTexture("earth", "assets/planets/earth.jpg");
    resourceManager.loadTexture("moon", "assets/planets/moon.jpg");
}

void SceneManager::createScene1() {
    Scene* scene1 = new Scene();
    scene1->registerLightingShader(resourceManager.getShaderProgram("blinn"));

    scene1->addLight(Light(
        0,
        glm::vec3(-0.3f, -1.0f, -0.2f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f)
    ));

    scene1->addLight(Light(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::radians(25.0f),
        glm::vec3(1.0f, 0.09f, 0.032f)
    ));

    {
        Scale* s = new Scale(0.5f, 0.5f, 0.5f);
        Tranform* pos = new Tranform(-3.0f, 0.0f, 0.0f);
        Transformation* trans = new Transformation();
        trans->addTrans(s);
        trans->addTrans(pos);

        DrawableObject* obj = new DrawableObject(
            *resourceManager.getModel("sphere"),
            *resourceManager.getShaderProgram("blinn"),
            *trans,
            glm::vec3(1.0f, 0.0f, 0.0f)
        );

        scene1->addObject(obj);
        drawableObjects.push_back(obj);
        transformations.push_back(trans);
        scales.push_back(s);
        tranforms.push_back(pos);
    }

    {
        Scale* s = new Scale(0.5f, 0.5f, 0.5f);
        Tranform* pos = new Tranform(0.0f, 0.0f, 0.0f);
        CustomTransform* customTrans = new CustomTransform(20.0f);

        Transformation* trans = new Transformation();
        trans->addTrans(s);
        trans->addTrans(pos);
        trans->addTrans(customTrans);

        DrawableObject* obj = new DrawableObject(
            *resourceManager.getModel("sphere"),
            *resourceManager.getShaderProgram("blinn"),
            *trans,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        scene1->addObject(obj);
        drawableObjects.push_back(obj);
        transformations.push_back(trans);
        scales.push_back(s);
        tranforms.push_back(pos);
    }

    {
        Scale* s = new Scale(0.5f, 0.5f, 0.5f);
        Tranform* pos = new Tranform(3.0f, 0.0f, 0.0f);
        Transformation* trans = new Transformation();
        trans->addTrans(s);
        trans->addTrans(pos);

        DrawableObject* obj = new DrawableObject(
            *resourceManager.getModel("sphere"),
            *resourceManager.getShaderProgram("blinn"),
            *trans,
            glm::vec3(0.0f, 0.0f, 1.0f)
        );

        scene1->addObject(obj);
        drawableObjects.push_back(obj);
        transformations.push_back(trans);
        scales.push_back(s);
        tranforms.push_back(pos);
    }

    addScene(scene1);
    ownedScenes.push_back(scene1);
}

void SceneManager::createScene2() {
    Scene* scene2 = new Scene();
    scene2->registerLightingShader(resourceManager.getShaderProgram("blinn"));

    std::vector<std::string> skyboxFaces = {
        "assets/sky/cubemap/posx.jpg",
        "assets/sky/cubemap/negx.jpg",
        "assets/sky/cubemap/posy.jpg",
        "assets/sky/cubemap/negy.jpg",
        "assets/sky/cubemap/posz.jpg",
        "assets/sky/cubemap/negz.jpg"
    };

    SkyBox* skybox = new SkyBox(skyboxFaces, resourceManager.getShaderProgram("skybox"));
    scene2->setSkyBox(skybox);

    scene2->addLight(Light(
        0,
        glm::vec3(-0.3f, -1.0f, -0.2f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 0.0f, 0.0f)
    ));

    arcadeGame = new ArcadeGame(
        scene2,
        resourceManager.getModel("sphere"),
        resourceManager.getShaderProgram("blinn")
    );

    for (int i = 0; i < 5; i++) {
        arcadeGame->spawnRandomTarget();
    }
    addScene(scene2);
    ownedScenes.push_back(scene2);
}

void SceneManager::createScene3() {
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
        if (!tree.transforms.empty()) {
            registerObjectTransform(tree.object, tree.transforms[0]);
        }
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

void SceneManager::createScene4() {
    Scene* scene4 = new Scene();
    scene4->registerLightingShader(resourceManager.getShaderProgram("phong_light"));

    // sun
    scene4->addLight(Light(
        1,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.95f, 0.85f),
        glm::vec3(1.0f, 0.027f, 0.0028f)
    ));

    Scale* sunScale = new Scale(1.5f, 1.5f, 1.5f);
    Tranform* sunPos = new Tranform(0.0f, 0.0f, 0.0f);
    Transformation* sunTrans = new Transformation();
    sunTrans->addTrans(sunScale);
    sunTrans->addTrans(sunPos);

    DrawableObject* sunObject = new DrawableObject(
        *resourceManager.getModel("koule"),
        *resourceManager.getShaderProgram("constant"),
        *sunTrans,
        glm::vec3(1.0f, 1.0f, 1.0f)  // ← BÍLÁ!
    );
    sunObject->setTexture(resourceManager.getTexture("sun"));
    scene4->addObject(sunObject);

    drawableObjects.push_back(sunObject);
    transformations.push_back(sunTrans);
    scales.push_back(sunScale);
    tranforms.push_back(sunPos);

    // earth
    earthOrbit = new OrbitTransform(
        8.0f,
        0.5f,
        2.0f,
        glm::vec3(0.0f, 0.0f, 0.0f)
    );

    Scale* earthScale = new Scale(0.6f, 0.6f, 0.6f);
    Transformation* earthTrans = new Transformation();
    earthTrans->addTrans(earthScale);
    earthTrans->addTrans(earthOrbit);

    DrawableObject* earthObject = new DrawableObject(
        *resourceManager.getModel("koule"),
        *resourceManager.getShaderProgram("phong_light"),
        *earthTrans,
        glm::vec3(1.0f, 1.0f, 1.0f)  // ← BÍLÁ!
    );
    earthObject->setTexture(resourceManager.getTexture("earth"));
    scene4->addObject(earthObject);

    drawableObjects.push_back(earthObject);
    transformations.push_back(earthTrans);
    scales.push_back(earthScale);

    // moon
    moonOrbit = new OrbitTransform(
        2.0f,
        1.5f,
        3.0f,
        earthOrbit->currentPosition
    );

    Scale* moonScale = new Scale(0.25f, 0.25f, 0.25f);
    Transformation* moonTrans = new Transformation();
    moonTrans->addTrans(moonScale);
    moonTrans->addTrans(moonOrbit);

    DrawableObject* moonObject = new DrawableObject(
        *resourceManager.getModel("koule"),
        *resourceManager.getShaderProgram("phong_light"),
        *moonTrans,
        glm::vec3(1.0f, 1.0f, 1.0f)  // ← BÍLÁ!
    );
    moonObject->setTexture(resourceManager.getTexture("moon"));
    scene4->addObject(moonObject);

    drawableObjects.push_back(moonObject);
    transformations.push_back(moonTrans);
    scales.push_back(moonScale);

    addScene(scene4);
    ownedScenes.push_back(scene4);
}

void SceneManager::addScene(Scene* scene) {
    scenes.push_back(scene);
}

void SceneManager::switchScene(int index) {
    if (index >= 0 && index < scenes.size()) {
        currentSceneIndex = index;

        if (objectManager) {
            objectManager->setScene(scenes[currentSceneIndex]);
            registerSceneObjectsToManager();
        }
    }
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

    if (currentSceneIndex == 1 && arcadeGame) {
        arcadeGame->update(dt);
    }

    if (currentSceneIndex == 3 && earthOrbit && moonOrbit) {
        earthOrbit->update(dt);
        moonOrbit->setOrbitCenter(earthOrbit->currentPosition);
        moonOrbit->update(dt);
    }
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

void SceneManager::setSelectedObject(int index) {
    if (index >= 0 && index < (int)drawableObjects.size()) {
        selectedObjectIndex = index;
        DrawableObject* obj = drawableObjects[index];
        std::cout << "Object selected: index=" << index << ", stencilID=" << obj->getStencilID() << std::endl;
    } else {
        selectedObjectIndex = -1;
        std::cout << "No object selected (invalid index: " << index << ")" << std::endl;
    }
}

DrawableObject* SceneManager::getObjectByIndex(int index) {
    if (index >= 0 && index < (int)drawableObjects.size()) {
        return drawableObjects[index];
    }
    return nullptr;
}

ArcadeGame* SceneManager::getArcadeGame() {
    return arcadeGame;
}
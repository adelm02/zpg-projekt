#ifndef ZPGPROJ_ARCADEGAME_H
#define ZPGPROJ_ARCADEGAME_H

#include "GameTarget.h"
#include "Scene.h"
#include "Texture.h"
#include <vector>
#include <iostream>

class ArcadeGame {
private:
    std::vector<GameTarget*> targets;
    Scene* gameScene;
    ShaderProgram* shader;
    Modell* appleModel;
    Modell* tomatoModel;
    Modell* bananModel;
    Modell* orangeModel;

    Texture* appleTexture;
    Texture* tomatoTexture;
    Texture* bananTexture;
    Texture* orangeTexture;

    int score;
    float spawnTimer;
    float spawnInterval;
    int maxTargets;

    std::vector<glm::vec3> spawnPositions;

public:
    ArcadeGame(Scene* scene, ShaderProgram* shaderProg,
               Modell* apple, Modell* tomato, Modell* banan, Modell* orange,
               Texture* appleTex, Texture* tomatoTex, Texture* bananTex, Texture* orangeTex)
            : gameScene(scene), shader(shaderProg),
              appleModel(apple), tomatoModel(tomato), bananModel(banan), orangeModel(orange),
              appleTexture(appleTex), tomatoTexture(tomatoTex), bananTexture(bananTex), orangeTexture(orangeTex),
              score(0), spawnTimer(0.0f), spawnInterval(1.5f), maxTargets(6) {

        std::srand(std::time(nullptr));

        // Grid 3x3
        spawnPositions = {
            glm::vec3(-3.0f, 0.0f, -12.0f), glm::vec3(0.0f, 0.0f, -12.0f), glm::vec3(3.0f, 0.0f, -12.0f),
            glm::vec3(-3.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(3.0f, 0.0f, -10.0f),
            glm::vec3(-3.0f, 0.0f, -8.0f),  glm::vec3(0.0f, 0.0f, -8.0f),  glm::vec3(3.0f, 0.0f, -8.0f)
        };
    }

    ~ArcadeGame() {
        for (auto* target : targets) {
            delete target;
        }
    }

    void update(float dt) {
        for (auto* target : targets) {
            target->update(dt);
        }

        targets.erase(
            std::remove_if(targets.begin(), targets.end(),
                [this](GameTarget* t) {
                    if (!t->active()) {
                        gameScene->removeObject(t->getVisual());
                        delete t;
                        return true;
                    }
                    return false;
                }),
            targets.end()
        );

        spawnTimer += dt;
        if (spawnTimer >= spawnInterval && (int)targets.size() < maxTargets) {
            spawnTimer = 0.0f;
            spawnRandomTarget();
        }
    }

    void spawnRandomTarget() {
        int posIndex = std::rand() % spawnPositions.size();
        glm::vec3 spawnPos = spawnPositions[posIndex];

        int movementType = std::rand() % 4;
        std::vector<glm::vec3> path;
        float speed;
        int points;
        float scale;
        glm::vec3 color = glm::vec3(1.0f);
        Modell* selectedModel = nullptr;
        Texture* selectedTexture = nullptr;

        //slow
        if (movementType == 0) {
            path.push_back(spawnPos);
            path.push_back(glm::vec3(spawnPos.x, 2.0f, spawnPos.z));
            path.push_back(spawnPos);
            speed = 0.4f;
            points = 5;
            scale = 4.f;
            selectedModel = appleModel;
            selectedTexture = appleTexture;
        }
        //quick
        else if (movementType == 1) {
            path.push_back(spawnPos);
            path.push_back(glm::vec3(spawnPos.x, 2.0f, spawnPos.z));
            path.push_back(spawnPos);
            speed = 1.2f;
            points = 20;
            scale = 2.f;
            selectedModel = tomatoModel;
            selectedTexture = tomatoTexture;
        }
        //diagonal
        else if (movementType == 2) {
            path.push_back(spawnPos);
            path.push_back(glm::vec3(spawnPos.x + 1.0f, 1.5f, spawnPos.z + 1.0f));
            path.push_back(glm::vec3(spawnPos.x - 1.0f, 1.5f, spawnPos.z - 1.0f));
            path.push_back(spawnPos);
            speed = 0.6f;
            points = 15;
            scale = 9.0f;
            selectedModel = bananModel;
            selectedTexture = bananTexture;
        }
        // zig-zagÍ
        else {
            path.push_back(spawnPos);
            path.push_back(glm::vec3(spawnPos.x - 0.8f, 1.0f, spawnPos.z));
            path.push_back(glm::vec3(spawnPos.x + 0.8f, 2.0f, spawnPos.z));
            path.push_back(spawnPos);
            speed = 0.7f;
            points = 10;
            scale = 0.2f;
            selectedModel = orangeModel;
            selectedTexture = orangeTexture;
        }

        GameTarget* target = new GameTarget(path[0], path, speed, points, scale, *selectedModel, *shader, color);

        if (selectedTexture) {
            target->getVisual()->setTexture(selectedTexture);
        }

        targets.push_back(target);
        gameScene->addObject(target->getVisual());
    }

    bool handleClick(const glm::vec3& worldPos) {
        std::cout << "Click at world position: (" << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << ")" << std::endl;


        for (size_t i = 0; i < targets.size(); i++) {
            GameTarget* target = targets[i];
            glm::vec3 targetPos = target->getPosition();
            float distance = glm::length(worldPos - targetPos);

            std::cout << "Target " << i << " at (" << targetPos.x << ", " << targetPos.y << ", " << targetPos.z << ") - Distance: " << distance << std::endl;

            if (target->checkHit(worldPos, 3.0f)) {
                score += target->getPoints();
                std::cout << "HIT +" << target->getPoints() << " points. Total: " << score << std::endl;
                target->hit();
                return true;
            }
        }

        std::cout << "MISS. Current score: " << score << std::endl;
        return false;
    }

    int getScore() const { return score; }
    int getActiveTargets() const { return targets.size(); }

    void reset() {
        for (auto* target : targets) {
            gameScene->removeObject(target->getVisual());
            delete target;
        }
        targets.clear();
        score = 0;
        spawnTimer = 0.0f;
    }
};

#endif //ZPGPROJ_ARCADEGAME_H
#ifndef ZPGPROJ_ARCADEGAME_H
#define ZPGPROJ_ARCADEGAME_H

#include "GameTarget.h"
#include "Scene.h"
#include <vector>
#include <iostream>

class ArcadeGame {
private:
    std::vector<GameTarget*> targets;
    Scene* gameScene;
    Modell* sphereModel;
    ShaderProgram* shader;

    int score;
    float spawnTimer;
    float spawnInterval;
    int maxTargets;

    std::vector<glm::vec3> spawnPositions;

public:
    ArcadeGame(Scene* scene, Modell* model, ShaderProgram* shaderProg)
        : gameScene(scene), sphereModel(model), shader(shaderProg),
          score(0), spawnTimer(0.0f), spawnInterval(1.0f), maxTargets(6) {
        std::srand(std::time(nullptr));

        // Grid 3x3
        spawnPositions = {
            glm::vec3(-3.0f, 0.0f, -12.0f),
            glm::vec3(0.0f, 0.0f, -12.0f),
            glm::vec3(3.0f, 0.0f, -12.0f),

            glm::vec3(-3.0f, 0.0f, -10.0f),
            glm::vec3(0.0f, 0.0f, -10.0f),
            glm::vec3(3.0f, 0.0f, -10.0f),

            glm::vec3(-3.0f, 0.0f, -8.0f),
            glm::vec3(0.0f, 0.0f, -8.0f),
            glm::vec3(3.0f, 0.0f, -8.0f)
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
        glm::vec3 color;

        // green - slow, easy
        if (movementType == 0) {
            path.push_back(spawnPos);
            path.push_back(glm::vec3(spawnPos.x, 2.0f, spawnPos.z));
            path.push_back(spawnPos);
            speed = 0.4f;
            points = 5;
            color = glm::vec3(0.2f, 1.0f, 0.2f);
        }
        // red - fast
        else if (movementType == 1) {
            path.push_back(spawnPos);
            path.push_back(glm::vec3(spawnPos.x, 2.0f, spawnPos.z));
            path.push_back(spawnPos);
            speed = 1.2f;
            points = 20;
            color = glm::vec3(1.0f, 0.2f, 0.2f);
        }
        // blue - diagonal
        else if (movementType == 2) {
            path.push_back(spawnPos);
            path.push_back(glm::vec3(spawnPos.x + 1.0f, 1.5f, spawnPos.z + 1.0f));
            path.push_back(glm::vec3(spawnPos.x - 1.0f, 1.5f, spawnPos.z - 1.0f));
            path.push_back(spawnPos);
            speed = 0.6f;
            points = 15;
            color = glm::vec3(0.2f, 0.5f, 1.0f);
        }
        // yellow - zigzag
        else {
            path.push_back(spawnPos);
            path.push_back(glm::vec3(spawnPos.x - 0.8f, 1.0f, spawnPos.z));
            path.push_back(glm::vec3(spawnPos.x + 0.8f, 2.0f, spawnPos.z));
            path.push_back(glm::vec3(spawnPos.x - 0.8f, 1.0f, spawnPos.z));
            path.push_back(spawnPos);
            speed = 0.7f;
            points = 10;
            color = glm::vec3(1.0f, 1.0f, 0.2f);
        }

        GameTarget* target = new GameTarget(path[0], path, speed, points,*sphereModel, *shader, color);

        targets.push_back(target);
        gameScene->addObject(target->getVisual());
    }

    bool handleClick(const glm::vec3& worldPos) {
        for (size_t i = 0; i < targets.size(); i++) {
            GameTarget* target = targets[i];

            if (target->checkHit(worldPos, 1.2f)) {
                score += target->getPoints();
                std::cout << "HIT! +" << target->getPoints() << " points. Total: " << score << std::endl;
                target->hit();
                return true;
            }
        }

        std::cout << "MISS! Current score: " << score << std::endl;
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
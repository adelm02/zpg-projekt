//
// GameTarget.h
//

#ifndef ZPGPROJ_GAMETARGET_H
#define ZPGPROJ_GAMETARGET_H

#include "DrawableObject.h"
#include "PathMovement.h"
#include "Scale.h"
#include "Transformation.h"
#include "Modell.h"
#include "ShaderProgram.h"
#include <vector>
#include <glm/glm.hpp>

class GameTarget {
private:
    DrawableObject* visualObject;
    PathMovement* pathMovement;
    Scale* scale;
    Transformation* transformation;
    int points;
    bool isActive;
    bool wasHit;

public:
    GameTarget(const glm::vec3& startPos, const std::vector<glm::vec3>& path,
               float speed, int pointValue, Modell& model, ShaderProgram& shader,
               glm::vec3 color)
        : points(pointValue), isActive(true), wasHit(false) {

        scale = new Scale(0.8f, 0.8f, 0.8f);
        pathMovement = new PathMovement(path, speed, false);

        transformation = new Transformation();
        transformation->addTrans(scale);
        transformation->addTrans(pathMovement);

        visualObject = new DrawableObject(model, shader, *transformation, color);
    }

    ~GameTarget() {
        delete visualObject;
        delete transformation;
        delete scale;
        delete pathMovement;
    }

    void update(float dt) {
        if (!isActive) return;
        pathMovement->update(dt);

        if (pathMovement->finished) {
            isActive = false;
        }
    }

    DrawableObject* getVisual() { return visualObject; }
    glm::vec3 getPosition() const { return pathMovement->currentPosition; }
    int getPoints() const { return points; }
    bool active() const { return isActive; }

    void hit() {
        wasHit = true;
        isActive = false;
    }

    bool checkHit(const glm::vec3& clickPos, float radius = 0.5f) {
        if (!isActive) return false;
        float distance = glm::length(clickPos - getPosition());
        return distance < radius;
    }
};

#endif
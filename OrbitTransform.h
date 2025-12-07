#ifndef ZPGPROJ_ORBITTRANSFORM_H
#define ZPGPROJ_ORBITTRANSFORM_H

#include "AbstractTrans.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class OrbitTransform : public AbstractTrans {
private:
    float orbitRadius;
    float orbitSpeed;
    float rotationSpeed;
    float currentOrbitAngle;
    float currentRotationAngle;
    glm::vec3 orbitCenter;
    bool isVertical;

public:
    glm::vec3 currentPosition;

    OrbitTransform(float radius, float orbSpeed, float rotSpeed,
                   const glm::vec3& center = glm::vec3(0.0f), bool isVertical = false) : orbitRadius(radius), orbitSpeed(orbSpeed), rotationSpeed(rotSpeed),currentOrbitAngle(0.0f), currentRotationAngle(0.0f),orbitCenter(center), isVertical(isVertical) {updatePosition();}

    glm::mat4 getMatrix() const override {
        //move to orbit
        glm::mat4 matrix = glm::translate(glm::mat4(1.0f), currentPosition);

        matrix = glm::rotate(matrix, currentRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));

        return matrix;
    }

    void apply() override {}

    void update(float dt) override {
        currentOrbitAngle += orbitSpeed * dt;

        if (currentOrbitAngle > 2.0f * 3.14159f) {
            currentOrbitAngle -= 2.0f * 3.14159f;
        }

        currentRotationAngle += rotationSpeed * dt;

        if (currentRotationAngle > 2.0f * 3.14159f) {
            currentRotationAngle -= 2.0f * 3.14159f;
        }

        updatePosition();
    }

    void setOrbitCenter(const glm::vec3& center) {
        orbitCenter = center;
        updatePosition();
    }

private:
    void updatePosition() {
        if (isVertical) {
            // Vertical
            float x = orbitCenter.x + orbitRadius * cos(currentOrbitAngle);
            float y = orbitCenter.y + orbitRadius * sin(currentOrbitAngle);
            currentPosition = glm::vec3(x, y, orbitCenter.z);
        } else {
            // Horizontal
            float x = orbitCenter.x + orbitRadius * cos(currentOrbitAngle);
            float z = orbitCenter.z + orbitRadius * sin(currentOrbitAngle);
            currentPosition = glm::vec3(x, orbitCenter.y, z);
        }
    }
};

#endif //ZPGPROJ_ORBITTRANSFORM_H
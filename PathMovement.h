
#ifndef ZPGPROJ_PATHMOVEMENT_H
#define ZPGPROJ_PATHMOVEMENT_H

#include "AbstractTrans.h"
#include <glm/glm.hpp>                      // ← DŮLEŽITÉ!
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class PathMovement : public AbstractTrans {
private:
    std::vector<glm::vec3> points;
    float speed;
    float currentTime;
    int currentSegment;
    bool loop;

public:
    glm::vec3 currentPosition;
    bool finished;

    PathMovement(const std::vector<glm::vec3>& pathPoints, float moveSpeed, bool shouldLoop = false)
        : points(pathPoints), speed(moveSpeed), currentTime(0.0f),
          currentSegment(0), loop(shouldLoop), finished(false) {
        if (!points.empty()) {
            currentPosition = points[0];
        }
    }

    glm::mat4 getMatrix() const override {
        return glm::translate(glm::mat4(1.0f), currentPosition);
    }

    void apply() override {}

    void update(float dt) override {
        if (finished && !loop) return;
        if (points.size() < 2) return;

        currentTime += dt * speed;

        while (currentTime >= 1.0f && currentSegment < (int)points.size() - 2) {
            currentTime -= 1.0f;
            currentSegment++;
        }

        if (currentSegment >= (int)points.size() - 2) {
            if (loop) {
                currentSegment = 0;
                currentTime = 0.0f;
            } else {
                finished = true;
                currentPosition = points.back();
                return;
            }
        }

        glm::vec3 start = points[currentSegment];
        glm::vec3 end = points[currentSegment + 1];
        currentPosition = glm::mix(start, end, currentTime);
    }

    void reset() {
        currentTime = 0.0f;
        currentSegment = 0;
        finished = false;
        if (!points.empty()) {
            currentPosition = points[0];
        }
    }
};

#endif //ZPGPROJ_PATHMOVEMENT_H
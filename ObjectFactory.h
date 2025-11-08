#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "DrawableObject.h"
#include "Modell.h"
#include "ShaderProgram.h"
#include "Transformation.h"
#include "Scale.h"
#include "Tranform.h"
#include "Rotate.h"
#include <glm/glm.hpp>
#include <vector>


struct ObjectComponents {
    DrawableObject* object;
    std::vector<Scale*> scales;
    std::vector<Tranform*> transforms;
    std::vector<Rotate*> rotations;
    Transformation* transformation;
};

class ObjectFactory {
public:

    static ObjectComponents createTree(const glm::vec3& position, Modell& model, ShaderProgram& shader);

    static ObjectComponents createBush(const glm::vec3& position, Modell& model, ShaderProgram& shader);

    static ObjectComponents createCharacter(const glm::vec3& position, float rotationY, float scale, Modell& model, ShaderProgram& shader);

    static ObjectComponents createGroundPlane(const glm::vec3& position, const glm::vec3& scaleVec, Modell& model, ShaderProgram& shader);
};

#endif
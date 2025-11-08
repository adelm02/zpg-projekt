#ifndef FIREFLY_H
#define FIREFLY_H

#include "DrawableObject.h"
#include "Light.h"
#include "Modell.h"
#include "ShaderProgram.h"
#include "Transformation.h"
#include "Scale.h"
#include "Tranform.h"
#include <glm/glm.hpp>

class Firefly {
private:
    DrawableObject* visualObject;
    Light light;
    glm::vec3 position;

    Scale* scale;
    Tranform* transform;
    Transformation* transformation;

public:
    Firefly(const glm::vec3& pos, Modell& model, ShaderProgram& shader);
    ~Firefly();

    Light& getLight() { return light; }
    DrawableObject* getVisualObject() { return visualObject; }
    glm::vec3 getPosition() const { return position; }
    
    Scale* getScale() { return scale; }
    Tranform* getTransform() { return transform; }
    Transformation* getTransformation() { return transformation; }
};

#endif
#include "Firefly.h"

Firefly::Firefly(const glm::vec3& pos, Modell& model, ShaderProgram& shader)
    : position(pos), light(pos, glm::vec3(1.5f, 1.5f, 1.5f), glm::vec3(1.0f, 0.22f, 0.20f))
{

    scale = new Scale(0.12f, 0.12f, 0.12f);
    transform = new Tranform(pos.x, pos.y, pos.z);
    transformation = new Transformation();
    
    transformation->addTrans(scale);
    transformation->addTrans(transform);
    
//white ball
    visualObject = new DrawableObject(model, shader, *transformation, glm::vec3(1.0f, 1.0f, 1.0f));
}

Firefly::~Firefly() {
}
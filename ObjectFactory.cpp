#include "ObjectFactory.h"

ObjectComponents ObjectFactory::createTree(const glm::vec3& position, Modell& model, ShaderProgram& shader) {
    ObjectComponents components;
    
    Tranform* move = new Tranform(position.x, position.y, position.z);
    Transformation* t = new Transformation();
    t->addTrans(move);
    
    DrawableObject* tree = new DrawableObject(model, shader, *t, 
                                               glm::vec3(0.20f, 0.65f, 0.40f));
    tree->setMaterial(model.getMaterial());
    
    components.object = tree;
    components.transforms.push_back(move);
    components.transformation = t;
    
    return components;
}

ObjectComponents ObjectFactory::createBush(const glm::vec3& position, Modell& model, ShaderProgram& shader) {
    ObjectComponents components;
    
    Scale* s = new Scale(3.0f, 3.0f, 3.0f);
    Tranform* move = new Tranform(position.x, position.y, position.z);
    Transformation* t = new Transformation();
    t->addTrans(s);
    t->addTrans(move);
    
    DrawableObject* bush = new DrawableObject(model, shader, *t, glm::vec3(0.12f, 0.45f, 0.18f));
    
    components.object = bush;
    components.scales.push_back(s);
    components.transforms.push_back(move);
    components.transformation = t;
    
    return components;
}

ObjectComponents ObjectFactory::createCharacter(const glm::vec3& position, float rotationY, float scale, Modell& model, ShaderProgram& shader) {
    ObjectComponents components;
    
    Scale* s = new Scale(scale, scale, scale);
    Rotate* rot = new Rotate(rotationY, glm::vec3(0.0f, 1.0f, 0.0f));
    Tranform* pos = new Tranform(position.x, position.y, position.z);
    
    Transformation* t = new Transformation();
    t->addTrans(s);
    t->addTrans(rot);
    t->addTrans(pos);
    
    DrawableObject* character = new DrawableObject(model, shader, *t);
    character->setMaterial(model.getMaterial());
    
    components.object = character;
    components.scales.push_back(s);
    components.rotations.push_back(rot);
    components.transforms.push_back(pos);
    components.transformation = t;
    
    return components;
}

ObjectComponents ObjectFactory::createGroundPlane(const glm::vec3& position, const glm::vec3& scaleVec, Modell& model, ShaderProgram& shader) {
    ObjectComponents components;
    
    Scale* s = new Scale(scaleVec.x, scaleVec.y, scaleVec.z);
    Tranform* move = new Tranform(position.x, position.y, position.z);
    
    Transformation* t = new Transformation();
    t->addTrans(s);
    t->addTrans(move);
    
    DrawableObject* ground = new DrawableObject(model, shader, *t, glm::vec3(0.27f, 0.25f, 0.15f));
    
    components.object = ground;
    components.scales.push_back(s);
    components.transforms.push_back(move);
    components.transformation = t;
    
    return components;
}
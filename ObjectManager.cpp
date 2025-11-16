
#include "ObjectManager.h"
#include <iostream>


ObjectManager::ObjectManager() : selectedIndex(-1), currentScene(nullptr) {
    std::cout << "ObjectManager created" << std::endl;
}

ObjectManager::~ObjectManager() {
    clear();
}

void ObjectManager::selectByStencilID(int stencilID) {
    // stencil ID +1 from index
    int index = stencilID - 1;
    selectByIndex(index);
}

void ObjectManager::selectByIndex(int index) {
    // update color from before
    if (selectedIndex >= 0) {
        restoreObjectColor(selectedIndex);
    }

    if (index >= 0 && index < (int)objects.size()) {
        selectedIndex = index;
        highlightObject(index);

        std::cout << "Object selected: index=" << index << ", stencilID=" << objects[index].stencilID << std::endl;
    } else {
        selectedIndex = -1;
        std::cout << "Invalid selection index: " << index << std::endl;
    }
}

void ObjectManager::deselect() {
    if (selectedIndex >= 0) {
        restoreObjectColor(selectedIndex);
        std::cout << "Object deselected: index=" << selectedIndex << std::endl;
        selectedIndex = -1;
    }
}

DrawableObject* ObjectManager::getSelectedObject() {
    if (selectedIndex >= 0 && selectedIndex < (int)objects.size()) {
        return objects[selectedIndex].object;
    }
    return nullptr;
}


void ObjectManager::addObject(ObjectData data) {
    if (!data.object) {
        std::cout << "Error: Cannot add null object!" << std::endl;
        return;
    }

    objects.push_back(data);
    int index = objects.size() - 1;

    // add to scene
    if (currentScene) {
        currentScene->addObject(data.object);
    }

    updateStencilIDs();
}

void ObjectManager::addObjectWithoutScene(ObjectData data) {
    if (!data.object) {
        std::cout << "Error: Cannot add null object!" << std::endl;
        return;
    }

    objects.push_back(data);
    updateStencilIDs();
}


void ObjectManager::deleteSelected() {
    if (selectedIndex >= 0) {
        deleteByIndex(selectedIndex);
    } else {
        std::cout << "No object selected to delete!" << std::endl;
    }
}

void ObjectManager::deleteByIndex(int index) {
    if (index < 0 || index >= (int)objects.size()) {
        std::cout << "Invalid index for deletion: " << index << std::endl;
        return;
    }

    if (currentScene) {
        currentScene->removeObject(objects[index].object);
    }

    // delete trans from vector
    objects.erase(objects.begin() + index);

    // update choice
    if (selectedIndex == index) {
        selectedIndex = -1;
    } else if (selectedIndex > index) {
        selectedIndex--;
    }

    updateStencilIDs();

    std::cout << "Object deleted. Remaining: " << objects.size() << std::endl;
}

void ObjectManager::clear() {
    objects.clear();
    selectedIndex = -1;
}

void ObjectManager::moveSelected(const glm::vec3& offset) {
    if (selectedIndex < 0 || selectedIndex >= (int)objects.size()) {
        return;
    }

    Tranform* transform = objects[selectedIndex].transform;
    if (!transform) {
        std::cout << "Cannot move object, no transform data" << std::endl;
        return;
    }

    transform->x += offset.x;
    transform->y += offset.y;
    transform->z += offset.z;
}

void ObjectManager::rotateSelected(float angleDelta, const glm::vec3& axis) {
    if (selectedIndex < 0 || selectedIndex >= (int)objects.size())
        return;

    // !rotate, create rotate
    if (!objects[selectedIndex].rotation) {
        if (!objects[selectedIndex].transformation) {
            std::cout << "Cannot rotate object, no transformation data" << std::endl;
            return;
        }

        objects[selectedIndex].rotation = new Rotate(0.0f, axis);
        objects[selectedIndex].transformation->addTrans(objects[selectedIndex].rotation);
    }

    objects[selectedIndex].rotation->angle += angleDelta;
}


void ObjectManager::scaleSelected(float factor) {
    if (selectedIndex < 0 || selectedIndex >= (int)objects.size())
        return;

    Scale* scale = objects[selectedIndex].scale;

    if (!scale) {
        std::cout << "Cannot scale object, no scale data" << std::endl;
        return;
    }

    scale->x *= factor;
    scale->y *= factor;
    scale->z *= factor;
}

void ObjectManager::updateStencilIDs() {
    for (size_t i = 0; i < objects.size(); i++) {
        objects[i].stencilID = i + 1; // Stencil ID from 1
        objects[i].object->setStencilID(i + 1);
    }
}

glm::vec3 ObjectManager::getObjectPosition(int index) const {
    if (index >= 0 && index < (int)objects.size()) {
        Tranform* transform = objects[index].transform;
        if (transform) {
            return glm::vec3(transform->x, transform->y, transform->z);
        }
    }
    return glm::vec3(0.0f);
}

void ObjectManager::highlightObject(int index) {
    if (index >= 0 && index < (int)objects.size()) {
        // before color
        objects[index].originalColor = objects[index].object->color;
        // yellow if picked
        objects[index].object->color = SELECTION_COLOR;
    }
}

void ObjectManager::restoreObjectColor(int index) {
    if (index >= 0 && index < (int)objects.size()) {
        objects[index].object->color = objects[index].originalColor;
    }
}
//
// ObjectManager.cpp - ZJEDNODUŠENÁ IMPLEMENTACE
// Pouze správa objektů
//

#include "ObjectManager.h"
#include <iostream>
#include <algorithm>

ObjectManager::ObjectManager() : selectedIndex(-1), currentScene(nullptr) {
    std::cout << "ObjectManager created" << std::endl;
}

ObjectManager::~ObjectManager() {
    clear();
}

// ============================================================================
// VÝBĚR OBJEKTŮ
// ============================================================================

void ObjectManager::selectByStencilID(int stencilID) {
    // Stencil ID je o 1 větší než index
    int index = stencilID - 1;
    selectByIndex(index);
}

void ObjectManager::selectByIndex(int index) {
    // Obnovit barvu předchozího objektu
    if (selectedIndex >= 0) {
        restoreObjectColor(selectedIndex);
    }

    if (index >= 0 && index < (int)objects.size()) {
        selectedIndex = index;
        highlightObject(index);

        std::cout << "Object selected: index=" << index
                  << ", stencilID=" << objects[index].stencilID << std::endl;
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

// ============================================================================
// PŘIDÁVÁNÍ OBJEKTŮ
// ============================================================================


void ObjectManager::addObject(ObjectData data) {
    if (!data.object) {
        std::cout << "Error: Cannot add null object!" << std::endl;
        return;
    }

    objects.push_back(data);
    int index = objects.size() - 1;

    // Přidat do scény
    if (currentScene) {
        currentScene->addObject(data.object);
    }

    // Aktualizovat stencil ID
    updateStencilIDs();

    std::cout << "Object added at index " << index
              << ", total objects: " << objects.size() << std::endl;
}

void ObjectManager::addObjectWithoutScene(ObjectData data) {
    if (!data.object) {
        std::cout << "Error: Cannot add null object!" << std::endl;
        return;
    }

    objects.push_back(data);

    // Aktualizovat stencil ID
    updateStencilIDs();
}

// ============================================================================
// MAZÁNÍ OBJEKTŮ
// ============================================================================

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

    std::cout << "Deleting object at index " << index << std::endl;

    // Odstranit ze scény
    if (currentScene) {
        currentScene->removeObject(objects[index].object);
    }

    // 🆕 UPRAVENO: Nemažeme transformace (spravuje je SceneManager)
    // Objekty vytvořené přes SceneManager mají své transformace v jeho vectorech
    // Zde jen odstraníme z ObjectManager sledování

    // Odstranit z vektoru
    objects.erase(objects.begin() + index);

    // Aktualizovat výběr
    if (selectedIndex == index) {
        selectedIndex = -1;
    } else if (selectedIndex > index) {
        selectedIndex--;
    }

    // Aktualizovat stencil IDs
    updateStencilIDs();

    std::cout << "Object deleted. Remaining: " << objects.size() << std::endl;
}

void ObjectManager::clear() {
    std::cout << "Clearing ObjectManager..." << std::endl;

    objects.clear();
    selectedIndex = -1;

    std::cout << "ObjectManager cleared" << std::endl;
}
// ============================================================================
// MANIPULACE S OBJEKTY
// ============================================================================

void ObjectManager::moveSelected(const glm::vec3& offset) {
    if (selectedIndex < 0 || selectedIndex >= (int)objects.size()) {
        return;
    }

    Tranform* transform = objects[selectedIndex].transform;
    if (!transform) {
        std::cout << "Cannot move object - no transform data" << std::endl;
        return;
    }

    transform->x += offset.x;
    transform->y += offset.y;
    transform->z += offset.z;

    std::cout << "Object moved to ("
              << transform->x << ", "
              << transform->y << ", "
              << transform->z << ")" << std::endl;
}

void ObjectManager::rotateSelected(float angleDelta, const glm::vec3& axis) {
    if (selectedIndex < 0 || selectedIndex >= (int)objects.size()) {
        return;
    }

    if (!objects[selectedIndex].rotation) {
        std::cout << "Cannot rotate object - no rotation data" << std::endl;
        return;
    }

    objects[selectedIndex].rotation = new Rotate(0.0f, axis);
    objects[selectedIndex].transformation->addTrans(objects[selectedIndex].rotation);


    objects[selectedIndex].rotation->angle += angleDelta;

    std::cout << "Object rotated to "
              << objects[selectedIndex].rotation->angle
              << " degrees" << std::endl;
}

void ObjectManager::scaleSelected(float factor) {
    if (selectedIndex < 0 || selectedIndex >= (int)objects.size()) {
        return;
    }

    Scale* scale = objects[selectedIndex].scale;

    if (!objects[selectedIndex].rotation) {
        std::cout << "Cannot rotate object - no rotation data" << std::endl;
        return;
    }

        scale->x *= factor;
        scale->y *= factor;
        scale->z *= factor;

        std::cout << "Object scaled to " << scale->x << std::endl;
}

// ============================================================================
// SPRÁVA SCÉNY
// ============================================================================

void ObjectManager::updateStencilIDs() {
    for (size_t i = 0; i < objects.size(); i++) {
        objects[i].stencilID = i + 1; // Stencil ID začíná od 1
        objects[i].object->setStencilID(i + 1);
    }
}

// ============================================================================
// INFORMACE
// ============================================================================

glm::vec3 ObjectManager::getObjectPosition(int index) const {
    if (index >= 0 && index < (int)objects.size()) {
        Tranform* transform = objects[index].transform;
        if (transform) {
            return glm::vec3(transform->x, transform->y, transform->z);
        }
    }
    return glm::vec3(0.0f);
}

// ============================================================================
// POMOCNÉ METODY
// ============================================================================

void ObjectManager::highlightObject(int index) {
    if (index >= 0 && index < (int)objects.size()) {
        // Uložit původní barvu
        objects[index].originalColor = objects[index].object->color;
        // Nastavit žlutou
        objects[index].object->color = SELECTION_COLOR;
    }
}

void ObjectManager::restoreObjectColor(int index) {
    if (index >= 0 && index < (int)objects.size()) {
        objects[index].object->color = objects[index].originalColor;
    }
}
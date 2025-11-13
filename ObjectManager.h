//
// ObjectManager.h - ZJEDNODUŠENÁ VERZE
// Pouze správa objektů - bez input handling
//

#ifndef ZPGPROJ_OBJECTMANAGER_H
#define ZPGPROJ_OBJECTMANAGER_H

#include <vector>
#include <glm/glm.hpp>
#include "DrawableObject.h"
#include "Scene.h"
#include "Transformation.h"
#include "Scale.h"
#include "Tranform.h"
#include "Rotate.h"

// one object
struct ObjectData {
    DrawableObject* object;
    Transformation* transformation;
    Scale* scale;
    Tranform* transform;
    Rotate* rotation;
    glm::vec3 originalColor;
    int stencilID;

    ObjectData() : object(nullptr), transformation(nullptr), scale(nullptr), transform(nullptr), rotation(nullptr), originalColor(0.0f), stencilID(0) {}};

class ObjectManager {
private:
    std::vector<ObjectData> objects;
    int selectedIndex;
    Scene* currentScene;

    const glm::vec3 SELECTION_COLOR = glm::vec3(1.0f, 1.0f, 0.0f); // Žlutá

public:
    ObjectManager();
    ~ObjectManager();

    // pick obj
    void selectByStencilID(int stencilID);
    void selectByIndex(int index);
    void deselect();
    bool hasSelection() const { return selectedIndex >= 0; }
    int getSelectedIndex() const { return selectedIndex; }
    DrawableObject* getSelectedObject();


    void addObject(ObjectData data);
    void addObjectWithoutScene(ObjectData data);

    void deleteSelected();
    void deleteByIndex(int index);
    void clear();

    void moveSelected(const glm::vec3& offset);
    void rotateSelected(float angleDelta, const glm::vec3& axis);
    void scaleSelected(float factor);

    void setScene(Scene* scene) { currentScene = scene; }
    void updateStencilIDs();

    int getObjectCount() const { return objects.size(); }
    const std::vector<ObjectData>& getObjects() const { return objects; }
    glm::vec3 getObjectPosition(int index) const;

private:
    void highlightObject(int index);
    void restoreObjectColor(int index);
};

#endif //ZPGPROJ_OBJECTMANAGER_H
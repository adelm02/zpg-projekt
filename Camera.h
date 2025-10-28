//
// Created by Adéla Martynková on 09.10.2025.
//

#ifndef ZPGPROJ_CAMERA_H
#define ZPGPROJ_CAMERA_H
#include <vector>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include "Observer.h"


class Camera {

protected:
    Camera();
    static Camera* instance;
    glm::vec3 CameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);
    glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    std::vector<Observer*> observers;
    bool firstMouse = true;
    float lastX = 0.0f;
    float lastY = 0.0f;
    float yaw = -90.0f;
    float pitch = 0.0f;
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    float aspect = 800.0f / 600.0f;




public:
    glm::mat4 getCamera();
    glm::mat4 Project;
    glm::mat4 View;
    Camera(Camera &other) = delete;
    void operator=(const Camera &) = delete;
    static Camera* getInstance();
    void addObserver(Observer *observer);
    void notify();
    glm::mat4 getProjection();
    glm::vec3 getCameraPos();
    void inputs(GLFWwindow* window, int key, int action);
    void camera_move(GLFWwindow* window, double xpos, double ypos);
    void setAspect(float a) { aspect = a; }
    void setFovDegrees(float f);
    float fovDeg = 60.0f;


};


#endif //ZPGPROJ_CAMERA_H
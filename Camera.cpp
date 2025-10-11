//
// Created by Adéla Martynková on 09.10.2025.
//

#include "Camera.h"
#include <iostream>

Camera* Camera::instance = nullptr;

glm::mat4 Camera::getCamera(){
    return glm::lookAt(CameraPosition, CameraPosition + CameraFront, CameraUp);
}

Camera * Camera::getInstance() {
    if (instance == nullptr) {
        instance = new Camera();
    }
    return instance;
}

void Camera::addObserver(Observer *observer) {
    observers.push_back(observer);
}

void Camera::notify() {
    for (Observer* observer : observers) {
        observer->update(this->getCamera(), this->getCameraPos(), this->getProjection());
    }
}

glm::mat4 Camera::getProjection() {
    return glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);
}

glm::vec3 Camera::getCameraPos() {
    return this->CameraPosition;
}

void Camera::inputs(GLFWwindow *window, int key, int action) {
    const float cameraSpeed = 0.5f;
    if (key==GLFW_KEY_W && (action==GLFW_PRESS || action==GLFW_REPEAT)) {
        printf("Front : %f, %f , %f\n", CameraFront.x, CameraFront.y, CameraFront.z);
        this->CameraPosition += cameraSpeed * this->CameraFront;
    }
    if (key==GLFW_KEY_S && (action==GLFW_PRESS || action==GLFW_REPEAT))
        this->CameraPosition -= cameraSpeed * this->CameraFront;
    if (key==GLFW_KEY_A && (action==GLFW_PRESS || action==GLFW_REPEAT))
        this->CameraPosition -= glm::normalize(glm::cross(this->CameraFront, this->CameraUp)) * cameraSpeed;
    if (key==GLFW_KEY_D && (action==GLFW_PRESS || action==GLFW_REPEAT))
        this->CameraPosition += glm::normalize(glm::cross(this->CameraFront, this->CameraUp)) * cameraSpeed;
}

void Camera::camera_move(GLFWwindow *window, double xpos, double ypos) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS)
        return;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    CameraFront = glm::normalize(direction);
    this->notify();
}


Camera::Camera() {
}

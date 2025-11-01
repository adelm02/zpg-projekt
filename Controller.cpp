//
// Created by Adéla Martynková on 11.10.2025.
//

#include "Controller.h"

#include <iostream>

#include "Camera.h"


Controller::Controller(GLFWwindow *window) {
    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetCursorPosCallback(window,mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void Controller::keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    Camera::getInstance()->inputs(window, key, action);
    Camera::getInstance()->notify();
}

void Controller::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    Camera::getInstance()->camera_move(window, xpos, ypos);
}



void Controller::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::cout << "Left tlacitko - choosen object" << std::endl;
    }
}
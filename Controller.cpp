//
// Created by Adéla Martynková on 11.10.2025.
//

#include "Controller.h"

#include <iostream>

#include "Camera.h"


Controller::Controller(GLFWwindow *window) {
    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetCursorPosCallback(window,mouse_callback);
}

void Controller::keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    Camera::getInstance()->inputs(window, key, action);
    Camera::getInstance()->notify();
}

void Controller::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    Camera::getInstance()->camera_move(window, xpos, ypos);

}

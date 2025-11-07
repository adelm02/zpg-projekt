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

    if (button == GLFW_MOUSE_BUTTON_RIGHT) { // Zkontrolujte, zda je tlačítko pravé

        if (action == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            // NUTNÉ: Říct kameře, že příští pohyb bude "první"
            Camera::getInstance()->setFirstMouse(true);

        } else if (action == GLFW_RELEASE) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            // NUTNÉ: I po puštění říct, že příští stisk zahájí "první" pohyb
            Camera::getInstance()->setFirstMouse(true);
        }
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::cout << "Left tlacitko - choosen object" << std::endl;
    }
}
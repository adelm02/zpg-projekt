//
// Created by Adéla Martynková on 11.10.2025.
//

#ifndef ZPGPROJ_CONTROLLER_H
#define ZPGPROJ_CONTROLLER_H
#include <GLFW/glfw3.h>


class Controller {
public:
    Controller(GLFWwindow* window);
    static void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};


#endif //ZPGPROJ_CONTROLLER_H
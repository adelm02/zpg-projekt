
#ifndef ZPGPROJ_CONTROLLER_H
#define ZPGPROJ_CONTROLLER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class SceneManager; // Forward declaration

class Controller {
private:
    static SceneManager* sceneManager;

public:
    Controller(GLFWwindow* window);
    static void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

    // Nastavení reference na SceneManager pro picking
    static void setSceneManager(SceneManager* manager);
};

#endif //ZPGPROJ_CONTROLLER_H
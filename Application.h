#ifndef ZPGPROJ_APPLICATION_H
#define ZPGPROJ_APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include "Scene.h"
#include "SceneManager.h"

class Application {
private:
    GLFWwindow* window;
    SceneManager& manager;
    double lastTime = 0.0;

public:
    Application(SceneManager& manager);
    ~Application();


    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void window_focus_callback(GLFWwindow* window, int focused);
    static void window_iconify_callback(GLFWwindow* window, int iconified);
    static void window_size_callback(GLFWwindow* window, int width, int height);

    void Run();
    bool init(int width, int height, const char* title);
    void terminate();

    GLFWwindow* getWindow() const { return window; }
};

#endif //ZPGPROJ_APPLICATION_H
#include "Application.h"
#include <cstdio>

Application::Application(SceneManager& manager) : manager(manager), window(nullptr) {}

Application::~Application() {
    terminate();
}

bool Application::init(int width, int height, const char* title) {
    glfwSetErrorCallback(Application::error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    lastTime = glfwGetTime();
    window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (!window) {
        glfwTerminate();
        return false;
    }

    Controller *controller = new Controller(window);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    glewInit();

    return true;
}

void Application::terminate() {
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}


void Application::error_callback(int error, const char* description) {
    fputs(description, stderr);
}


void Application::window_focus_callback(GLFWwindow* window, int focused) {
    printf("window_focus_callback \n");
}

void Application::window_iconify_callback(GLFWwindow* window, int iconified) {
    printf("window_iconify_callback \n");
}

void Application::window_size_callback(GLFWwindow* window, int width, int height) {
    printf("resize %d, %d\n", width, height);
    glViewport(0, 0, width, height);
}

void Application::Run() {

    lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) manager.switchScene(0);
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) manager.switchScene(1);
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) manager.switchScene(2);

        // Vykresli aktuální scénu
        manager.drawCurrentScene();
        manager.update(deltaTime);

        glEnable(GL_DEPTH_TEST);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

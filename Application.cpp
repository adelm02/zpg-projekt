#include "Application.h"
#include <cstdio>
#include "Light.h"
#include "Tranform.h"

extern Tranform moveEarth;
extern Tranform moveMoon;

Application::Application(SceneManager& manager) : manager(manager), window(nullptr),
    flashlightOn(false), fKeyPressed(false) {}

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

    // DŮLEŽITÉ: Povolit stencil buffer (8 bitů)
    glfwWindowHint(GLFW_STENCIL_BITS, 8);

    lastTime = glfwGetTime();
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    glfwSetFramebufferSizeCallback(window, Application::window_size_callback);

    if (!window) {
        glfwTerminate();
        return false;
    }

    Controller *controller = new Controller(window);

    // Nastavit SceneManager pro Controller (pro picking)
    Controller::setSceneManager(&manager);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    glewInit();

    // Povolit depth test a stencil test
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

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

    float aspect = (height > 0) ? (float)width / (float)height : 1.0f;
    Camera::getInstance()->setAspect(aspect);
}

void Application::Run() {
    lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Vyčistit color, depth i stencil buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        static float angleE = 0.0f;
        static float angleM = 0.0f;

        const float speedE = 0.8f;
        const float speedM = 2.0f;
        const float radiusE = 2.5f;
        const float radiusM = 3.5f;

        angleE += (float)deltaTime * speedE;
        angleM += (float)deltaTime * speedM;

        float earthX = cosf(angleE) * radiusE;
        float earthZ = sinf(angleE) * radiusE;
        float moonX  = earthX + cosf(angleM) * radiusM;
        float moonZ  = earthZ + sinf(angleM) * radiusM;

        moveEarth = Tranform(earthX, 0.0f, earthZ);
        moveMoon  = Tranform(moonX,  0.0f, moonZ);

        // Přepínání scén
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) manager.switchScene(0);
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) manager.switchScene(1);
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) manager.switchScene(2);
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) manager.switchScene(3);
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) manager.switchScene(4);

        // FOV změny
        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
            Camera::getInstance()->setFovDegrees(45.0f);
            printf("FOV set to 45 deg\n");
        }
        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
            Camera::getInstance()->setFovDegrees(90.0f);
            printf("FOV set to 90 deg\n");
        }
        if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
            Camera::getInstance()->setFovDegrees(130.0f);
            printf("FOV set to 130 deg\n");
        }

        // Flashlight toggle
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !fKeyPressed) {
            flashlightOn = !flashlightOn;
            fKeyPressed = true;
            printf("Flashlight: %s\n", flashlightOn ? "On" : "Off");
        }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) {
            fKeyPressed = false;
        }

        updateFlashlight();

        // Vykreslit scénu SE STENCIL BUFFEREM
        Scene* currentScene = manager.getCurrentScene();
        if (currentScene) {
            currentScene->drawAllWithStencil();
        }

        manager.update(deltaTime);
        glEnable(GL_DEPTH_TEST);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void Application::updateFlashlight() {
    glm::vec3 camPos = Camera::getInstance()->getCameraPos();
    glm::vec3 camDir = Camera::getInstance()->getDirection();

    Light flashlight(
        camPos, camDir,
        flashlightOn ? glm::vec3(3.0f, 3.0f, 2.8f) : glm::vec3(0.0f),
        glm::radians(30.0f),
        glm::vec3(1.0f, 0.09f, 0.032f)
    );

    Scene* currentScene = manager.getCurrentScene();
    if (currentScene) {
        int sceneIndex = manager.getCurrentSceneIndex();
        if (sceneIndex == 0) {
            currentScene->updateLight(1, flashlight);
        }
        else if (sceneIndex == 2) {
            currentScene->updateLight(6, flashlight);
        }
    }
}
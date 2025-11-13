//
// Controller.cpp
// Upraveno pro ObjectManager
//

#include "Controller.h"
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "SceneManager.h"

// Static member initialization
SceneManager* Controller::sceneManager = nullptr;

Controller::Controller(GLFWwindow *window) {
    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void Controller::keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // === KAMERA (původní) ===
    Camera::getInstance()->inputs(window, key, action);
    Camera::getInstance()->notify();

    // === OBJEKTY (pouze mazání a deselect) ===
    if (!sceneManager) return;
    if (action != GLFW_PRESS && action != GLFW_REPEAT) return;

    ObjectManager* objMgr = sceneManager->getObjectManager();
    if (!objMgr) return;

    // MAZÁNÍ OBJEKTU - DELETE nebo BACKSPACE
    if (key == GLFW_KEY_DELETE || key == GLFW_KEY_BACKSPACE) {
        if (objMgr->hasSelection()) {
            std::cout << "Deleting selected object..." << std::endl;
            objMgr->deleteSelected();
        } else {
            std::cout << "No object selected to delete!" << std::endl;
        }
        return;
    }

    // ZRUŠENÍ VÝBĚRU - ESC
    if (key == GLFW_KEY_ESCAPE) {
        objMgr->deselect();
        return;
    }

    // 🆕 SMAZÁN kód pro pohyb/rotaci/škálování
}

void Controller::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    // Kamera (původní)
    Camera::getInstance()->camera_move(window, xpos, ypos);
}

void Controller::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    // === PRAVÉ TLAČÍTKO - KAMERA (původní) ===
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            Camera::getInstance()->setFirstMouse(true);
        } else if (action == GLFW_RELEASE) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            Camera::getInstance()->setFirstMouse(true);
        }
    }

    // === LEVÉ TLAČÍTKO - OBJECT PICKING (upravené) ===
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {

        // Pozice kurzoru
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Velikost okna
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        // Převod souřadnic (OpenGL má origin vlevo dole)
        GLint x = (GLint)xpos;
        GLint y = height - (GLint)ypos;

        std::cout << "Click at screen coordinates: (" << xpos << ", " << ypos << ")" << std::endl;

        // Data z bufferů
        GLbyte color[4];
        GLfloat depth;
        GLuint stencilIndex;

        // Čtení color, depth, stencil
        glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
        glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
        glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &stencilIndex);

        // Debug výpis
        std::cout << "Clicked on pixel: " << x << ", " << y
                  << " Color: R=" << (int)color[0] << ", G=" << (int)color[1]
                  << ", B=" << (int)color[2] << ", A=" << (int)color[3] << std::endl;
        std::cout << "Depth: " << depth << std::endl;
        std::cout << "Stencil index: " << stencilIndex << std::endl;

        // Výpočet 3D pozice pomocí unProject
        glm::vec3 screenPos = glm::vec3(x, y, depth);
        glm::mat4 view = Camera::getInstance()->getCamera();
        glm::mat4 projection = Camera::getInstance()->getProjection();
        glm::vec4 viewport = glm::vec4(0, 0, width, height);
        glm::vec3 worldPos = glm::unProject(screenPos, view, projection, viewport);

        std::cout << "unProject position: (" << worldPos.x << ", "
                  << worldPos.y << ", " << worldPos.z << ")" << std::endl;

        // === NOVÉ: Použití ObjectManager ===
        if (sceneManager) {
            ObjectManager* objMgr = sceneManager->getObjectManager();
            if (objMgr) {
                if (stencilIndex == 0) {
                    // Kliknutí na pozadí - zrušit výběr
                    std::cout << "Clicked on background - deselecting" << std::endl;
                    objMgr->deselect();
                } else if (stencilIndex > 0 && stencilIndex <= 255) {
                    // Kliknutí na objekt - vybrat ho
                    std::cout << "Selected object with stencil ID: " << stencilIndex << std::endl;
                    objMgr->selectByStencilID(stencilIndex);
                }
            }
        }
    }
}

void Controller::setSceneManager(SceneManager* manager) {
    sceneManager = manager;
}
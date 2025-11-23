#include "Controller.h"
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "SceneManager.h"
#include "ArcadeGame.h"

SceneManager* Controller::sceneManager = nullptr;
bool Controller::isDragging = false;
glm::vec3 Controller::dragStartWorldPos = glm::vec3(0.0f);

Controller::Controller(GLFWwindow *window) {
    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void Controller::keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (!sceneManager) return;
    if (action != GLFW_PRESS && action != GLFW_REPEAT) return;

    ObjectManager* objMgr = sceneManager->getObjectManager();
    if (!objMgr) return;

    if ((mods & GLFW_MOD_SHIFT) && objMgr->hasSelection()) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) {
            const float moveSpeed = 0.5f;
            glm::vec3 offset(0.0f);

            if (key == GLFW_KEY_W) {
                offset.z = moveSpeed;
            }
            else if (key == GLFW_KEY_S) {
                offset.z = -moveSpeed;
            }
            else if (key == GLFW_KEY_A) {
                offset.x = -moveSpeed;
            }
            else if (key == GLFW_KEY_D) {
                offset.x = moveSpeed;
            }

            if (offset.x != 0.0f || offset.z != 0.0f) {
                sceneManager->moveSelectedObject(offset);
                std::cout << "Object moved by: (" << offset.x << ", " << offset.z << ")" << std::endl;
                return;
            }
        }
    }

    Camera::getInstance()->inputs(window, key, action);
    Camera::getInstance()->notify();

    if (key == GLFW_KEY_DELETE || key == GLFW_KEY_BACKSPACE) {
        if (objMgr->hasSelection()) {
            std::cout << "Deleting selected object" << std::endl;
            objMgr->deleteSelected();
        } else {
            std::cout << "No object selected to delete" << std::endl;
        }
        return;
    }

    if (key == GLFW_KEY_ESCAPE) {
        objMgr->deselect();
        return;
    }
}

void Controller::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    Camera::getInstance()->camera_move(window, xpos, ypos);
}

void Controller::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            Camera::getInstance()->setFirstMouse(true);
        } else if (action == GLFW_RELEASE) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            Camera::getInstance()->setFirstMouse(true);
        }
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        GLint x = (GLint)xpos;
        GLint y = height - (GLint)ypos;

        std::cout << "Click at screen coordinates: (" << xpos << ", " << ypos << ")" << std::endl;

        GLbyte color[4];
        GLfloat depth;
        GLuint stencilIndex;

        glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
        glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
        glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &stencilIndex);

        std::cout << "Clicked on pixel: " << x << ", " << y<< " Color: R=" << (int)color[0] << ", G=" << (int)color[1]<< ", B=" << (int)color[2] << ", A=" << (int)color[3] << std::endl;
        std::cout << "Depth: " << depth << std::endl;
        std::cout << "Stencil index: " << stencilIndex << std::endl;

        glm::vec3 screenPos = glm::vec3(x, y, depth);
        glm::mat4 view = Camera::getInstance()->getCamera();
        glm::mat4 projection = Camera::getInstance()->getProjection();
        glm::vec4 viewport = glm::vec4(0, 0, width, height);
        glm::vec3 worldPos = glm::unProject(screenPos, view, projection, viewport);

        std::cout << "unProject position: (" << worldPos.x << ", " << worldPos.y << ", " << worldPos.z << ")" << std::endl;

        if (sceneManager) {
            //arcade (scene2)
            if (sceneManager->getCurrentSceneIndex() == 1) {
                ArcadeGame* arcade = sceneManager->getArcadeGame();
                if (arcade) {
                    arcade->handleClick(worldPos);
                    return;
                }
            }

            ObjectManager* objMgr = sceneManager->getObjectManager();

            if (mods & GLFW_MOD_SHIFT) {
                sceneManager->plantTreeAt(worldPos);
                return;
            }

            if (objMgr) {
                if (stencilIndex == 0) {
                    objMgr->deselect();
                } else if (stencilIndex > 0 && stencilIndex <= 255) {
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
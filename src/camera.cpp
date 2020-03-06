#include <camera.hpp>

#include <iostream>

using namespace std;
using namespace MineyCrafty;

Camera::Camera()
    : Camera(glm::vec3(40.0f, 40.0f, 40.0f), glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f), 640, 480) { }

Camera::Camera(glm::vec3 pos, glm::vec3 up, glm::vec3 lookAt,
                int screenWidth, int screenHeight)
    : pos(pos), up(up), lookAt(lookAt), screenWidth(screenWidth),
      screenHeight(screenHeight), inputManager(util::InputManager::getInputManager()),
      timer(util::Timer::getTimer())
{
    view = glm::lookAt(pos, lookAt, up);
    proj = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
}

Camera::~Camera() { }

void Camera::update() {
    if (main == nullptr) {
        main = this->shared_from_this();
    }
    bool updated = false;
    if (inputManager->isKeyDown(SDL_SCANCODE_W)) {
        glm::vec3 fromPosToLook = lookAt - pos;
        pos += fromPosToLook * 0.5f * timer->deltaTime();
        updated = true;
    } else if (inputManager->isKeyDown(SDL_SCANCODE_S)) {
        glm::vec3 fromPosToLook = lookAt - pos;
        pos -= fromPosToLook * 0.5f * timer->deltaTime();
        updated = true;
    }
    if (inputManager->isKeyDown(SDL_SCANCODE_D)) {
        glm::vec3 left = glm::cross(lookAt - pos, up);
        glm::vec3 rightLookAt = glm::cross(pos - lookAt, up);
        pos += left * 0.5f * timer->deltaTime();
        lookAt -= rightLookAt * 0.5f * timer->deltaTime();
        updated = true;
    } else if (inputManager->isKeyDown(SDL_SCANCODE_A)) {
        glm::vec3 left = glm::cross(lookAt - pos, up);
        glm::vec3 rightLookAt = glm::cross(pos - lookAt, up);
        pos -= left * 0.5f * timer->deltaTime();
        lookAt += rightLookAt * 0.5f * timer->deltaTime();
        updated = true;
    }
    if (updated) {
        view = glm::lookAt(pos, lookAt, up);
    }
}

glm::mat4 Camera::getView() {
    return view;
}
glm::mat4 Camera::getProj() {
    return proj;
}
std::shared_ptr<Camera> Camera::getMainCamera() {
    if (main != nullptr) {
        return main;
    }
    return nullptr;
}
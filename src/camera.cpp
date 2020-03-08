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
      timer(util::Timer::getTimer()), mouse(util::Mouse::getMouse())
{
    float radiansYaw = glm::radians(yaw);
    float radiansPitch = glm::radians(pitch);
    lookAt = glm::vec3(
        cos(radiansYaw) * cos(radiansPitch),
        sin(radiansPitch),
        sin(radiansYaw) * cos(radiansPitch)
    );
    view = glm::lookAt(pos, pos + lookAt, up);
    proj = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
}

Camera::~Camera() { }

void Camera::update(int relx, int rely) {
    float deltaTime = timer->deltaTime();
    if (main == nullptr) {
        main = this->shared_from_this();
    }
    bool updated = false;
    if (relx != 0.0f && rely != 0.0f) {
        yaw += relx * sensitivity * deltaTime;
        pitch += rely * sensitivity * deltaTime;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        float radiansYaw = glm::radians(yaw);
        float radiansPitch = glm::radians(pitch);
        lookAt = glm::normalize(glm::vec3(
            cos(radiansYaw) * cos(radiansPitch),
            sin(radiansPitch),
            sin(radiansYaw) * cos(radiansPitch)
        ));
        updated = true;
    } else {
        if (inputManager->isKeyDown(SDL_SCANCODE_W)) {
            pos += lookAt * 10.0f * deltaTime;
            updated = true;
        } else if (inputManager->isKeyDown(SDL_SCANCODE_S)) {
            pos -= lookAt * 10.0f * deltaTime;
            updated = true;
        }
        if (inputManager->isKeyDown(SDL_SCANCODE_D)) {
            glm::vec3 rightLookAt = glm::normalize(glm::cross(lookAt, up));
            pos += rightLookAt * 10.0f * deltaTime;
            updated = true;
        } else if (inputManager->isKeyDown(SDL_SCANCODE_A)) {
            glm::vec3 rightLookAt = glm::normalize(glm::cross(lookAt, up));
            pos -= rightLookAt * 10.0f * deltaTime;
            updated = true;
        }
    }
    if (updated) {
        view = glm::lookAt(pos, pos + lookAt, up);
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

pair<int, int> Camera::getChunkPos() {
    return pair{floor(pos.x), floor(pos.z)};
}

glm::vec2 Camera::getChunkFacingDir() {
    auto facing = pos + lookAt;
    return glm::vec2(facing.x, facing.z);
}
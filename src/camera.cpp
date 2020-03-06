#include <camera.hpp>

#include <iostream>

using namespace std;
using namespace MineyCrafty;

/*
glm::mat4 view, proj;
    glm::vec3 pos, up;

public:
    Camera();
    Camera(glm::vec3 pos, glm::vec3 up, glm::vec3 lookAt);
    virtual ~Camera();

    void update();

    glm::mat4 getView();
    glm::mat4 getProj();
    static std::unique_ptr<Camera> getMainCamera();
*/

Camera::Camera()
    : Camera(glm::vec3(40.0f, 40.0f, 40.0f), glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 0.0f), 640, 480) { }

Camera::Camera(glm::vec3 pos, glm::vec3 up, glm::vec3 lookAt,
                int screenWidth, int screenHeight)
    : pos(pos), up(up), lookAt(lookAt), screenWidth(screenWidth),
      screenHeight(screenHeight) { }

Camera::~Camera() { }

void Camera::update() {
    if (main == nullptr) {
        main = this->shared_from_this();
    }
    view = glm::lookAt(pos, lookAt, up);
    proj = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
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
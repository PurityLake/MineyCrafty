#ifndef __HPP_CAMERA__
#define __HPP_CAMERA__

#include <memory>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <util/util.hpp>

namespace MineyCrafty {

class Camera : public std::enable_shared_from_this<Camera> {
private:
    glm::mat4 view, proj;
    glm::vec3 pos, up, lookAt;
    int screenWidth, screenHeight;
    float sensitivity = 2.0f;
    float yaw = 230.0f, pitch = 0.0f;
    
    std::shared_ptr<util::Mouse> mouse;
    std::shared_ptr<util::InputManager> inputManager;
    std::shared_ptr<util::Timer> timer;

    static inline std::shared_ptr<Camera> main = nullptr;

public:
    Camera();
    Camera(glm::vec3 pos, glm::vec3 up, glm::vec3 lookAt, int screenWidth, int screenHeight);
    virtual ~Camera();

    void update(int relX, int relY);

    glm::mat4 getView();
    glm::mat4 getProj();

    std::pair<int, int> getChunkPos();
    glm::vec2 getChunkFacingDir(); 

    static std::shared_ptr<Camera> getMainCamera();
};

} // namespace MineyCrafty

#endif /* __HPP_CAMERA__ */
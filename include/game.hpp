#ifndef __HPP_GAME__
#define __HPP_GAME__

#include <memory>

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

#ifndef __APPLE__
# include <GL/glu.h>
#else
# include <OpenGL/glu.h>
#endif

#include <camera.hpp>
#include <chunk.hpp>
#include <util/util.hpp>

#include <FastNoise.h>

namespace MineyCrafty {

class Game {
private:
    static constexpr int SCREEN_WIDTH = 1024;
    static constexpr int SCREEN_HEIGHT = 720;
    std::shared_ptr<Camera> cam;
    double deltaTime = 1.0;
    std::shared_ptr<util::Mouse> mouse;
    std::shared_ptr<util::InputManager> inputManager;
    std::shared_ptr<util::Timer> timer; 
    SDL_Window *window;
    SDL_GLContext context;
    std::vector<std::vector<Chunk>> chunks;
	GLuint  depthMapFBO, depthMap;
	glm::mat4 lightSpace;
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	float getNoise(FastNoise &noise, int x, int y);

    bool initSDL();
    void generateFrameBuffer();
    void generateQuadBuffer();

public:
    Game();
    virtual ~Game();

    void init();
    void render();
    void update();
    void loop();
    void finalise();
};

} // namespace MineyCrafty

#endif /* __HPP_GAME__ */
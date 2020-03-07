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

namespace MineyCrafty {

class Game {
private:
    static constexpr int SCREEN_WIDTH = 640;
    static constexpr int SCREEN_HEIGHT = 480;
    std::shared_ptr<Camera> cam;
    double deltaTime = 1.0;
    std::shared_ptr<util::Mouse> mouse;
    std::shared_ptr<util::InputManager> inputManager;
    std::shared_ptr<util::Timer> timer; 
    SDL_Window *window;
    SDL_GLContext context;
    std::vector<std::vector<Chunk>> chunks;

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
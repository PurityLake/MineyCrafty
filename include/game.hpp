#ifndef __HPP_GAME__
#define __HPP_GAME__

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>

#ifndef __APPLE__
# include <GL/glu.h>
#else
# include <OpenGL/glu.h>
#endif

#include <util.hpp>
#include <cube.hpp>

namespace MineyCrafty {

class Game {
private:
    static constexpr int SCREEN_WIDTH = 640;
    static constexpr int SCREEN_HEIGHT = 480;
    SDL_Window *window;
    SDL_GLContext context;
    Cube prim;

public:
    Game();
    virtual ~Game();

    void init();
    void render();
    void finalise();
};

} // namespace MineyCrafty

#endif /* __HPP_GAME__ */
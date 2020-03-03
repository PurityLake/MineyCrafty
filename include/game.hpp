#ifndef __HPP_GAME__
#define __HPP_GAME__

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <OpenGL/glu.h>

#include <rectprim.hpp>
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
};

}

#endif /* __HPP_GAME__ */
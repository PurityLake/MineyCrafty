#include <game.hpp>

#include <SDL_image.h>

#include <ctime>
#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace MineyCrafty;

Game::Game() {

}

Game::~Game() {

}

void Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialise! SDL_Error: " << SDL_GetError() << endl;
    } else {
        if (IMG_Init(IMG_INIT_PNG) < 0) {
            cerr << "SDL_image could not initialise! " << IMG_GetError() << endl;
        } else {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

            window = SDL_CreateWindow("MineyCrafty", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
            if (window == NULL) {
                cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
            } else {
                context = SDL_GL_CreateContext(window);
                if (context == NULL) {
                    cerr << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << endl;
                } else {
                    glewExperimental = GL_TRUE;
                    GLenum glewError = glewInit();
                    if (glewError != GLEW_OK) {
                        cerr << "Error initializing GLEW! " << glewGetErrorString(glewError) << endl;
                    }
                    if (SDL_GL_SetSwapInterval(1) < 0) {
                        cerr << "Warning: Unable to set VSync! SDL Error: %s\n" << SDL_GetError() << endl;
                    }
                    glEnable(GL_DEPTH_TEST);
                    glEnable(GL_TEXTURE_2D);
                    glEnable(GL_POLYGON_SMOOTH);
                    glDepthFunc(GL_LESS);
                    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                    inputManager = make_shared<util::InputManager>();
                    cam = make_shared<Camera>(glm::vec3(40.0f, 40.0f, 40.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 SCREEN_WIDTH, SCREEN_HEIGHT);
                    cam->update();
                    chunk.init();
                    int width = 10;
                    int height = 10;
                    util::NoiseVector v(width, height);
                    for (int z = 0; z < height; ++z) {
                        for (int x = 0; x < width; ++x) {
                            int height = floor(v.getAt(x, z) * 20);
                            for (int h = 0; h < height; ++h) {
                                chunk.addCube(x, h, z);
                            }
                        }
                    }
                    chunk.update();
                }
            }
        }
    }
}

void Game::render() {
    timer.start();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(
        glm::rotate(
            glm::translate(trans, glm::vec3(10, 10, 10)),
                ((float)M_PI / 4.0f) * (float)deltaTime,
                glm::vec3(0.0f, 1.0f, 0.0f)
        ), glm::vec3(-10, -10, -10));
    chunk.draw(trans);
    SDL_GL_SwapWindow(window);
    deltaTime = timer.deltaTime();
}

void Game::update() {
    cam->update();
}

void Game::loop() {
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }
        render();
        update();
    }
}

void Game::finalise() {
    IMG_Quit();
    SDL_Quit();
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;        
    }
    chunk.finalise();
}
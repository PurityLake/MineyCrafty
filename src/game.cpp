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
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                    glEnable(GL_DEPTH_TEST);
                    glEnable(GL_TEXTURE_2D);
                    glEnable(GL_POLYGON_SMOOTH);
                    glDepthFunc(GL_LESS);
                    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                    mouse = make_shared<util::Mouse>();
                    mouse->update();
                    inputManager = make_shared<util::InputManager>();
                    inputManager->init();
                    timer = make_shared<util::Timer>();
                    timer->start();
                    cam = make_shared<Camera>(glm::vec3(40.0f, 40.0f, 40.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 SCREEN_WIDTH, SCREEN_HEIGHT);
                    cam->update(0.0f, 0.0f);
                    int width = 10;
                    int height = 10;
                    chunks.reserve(20);
                    for (int chunkY = 1; chunkY <= 10; ++chunkY) {
                        std::vector<Chunk> chunkRow;
                        chunkRow.reserve(20); 
                        for (int chunkX = 1; chunkX <= 10; ++chunkX) {
                            Chunk chunk = Chunk(chunkX, chunkY);
                            chunk.init();
                            util::NoiseVector v(width, height, time(NULL));
                            for (int z = 0; z < Chunk::l; ++z) {
                                for (int x = 0; x < Chunk::w; ++x) {
                                    int height = floor(v.getAt(x, z) * 20);
                                    for (int h = 0; h < height; ++h) {
                                        chunk.addCube(x, h, z);
                                    }
                                }
                            }
                            chunk.update();
                            chunkRow.push_back(chunk);
                        }
                        chunks.push_back(chunkRow);
                    }
                }
            }
        }
    }
}

void Game::render() {
    timer->start();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static glm::mat4 trans = glm::mat4(1.0f);
    for (auto& row : chunks) {
        for (auto& c : row) {
            c.draw(trans);
        }
    }
    SDL_GL_SwapWindow(window);
    deltaTime = timer->deltaTime();
}

void Game::update() {
    cam->update(0.0f, 0.0f);
    mouse->update();
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
                case SDL_MOUSEMOTION:
                    cam->update(e.motion.xrel, e.motion.yrel);
                    break;
            }
        }
        if (inputManager->isKeyDown(SDL_SCANCODE_Q)) {
            break;
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
    for (auto& row : chunks) {
        for (auto& c : row) {
            c.finalise();
        }
    }
}
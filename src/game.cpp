#include <game.hpp>

#include <SDL_image.h>

#include <cmath>
#include <ctime>
#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace MineyCrafty;

static const GLfloat g_quad_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
};

static GLuint vao;
static GLuint quadVertexBuffer;
static Shader quadShader;

Game::Game() {

}

Game::~Game() {

}

float Game::getNoise(FastNoise& noise, int x, int y) {
	return noise.GetNoise(static_cast<FN_DECIMAL>(x), static_cast<FN_DECIMAL>(y)) / 2.0f + 0.5f;
}

void Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialise! SDL_Error: " << SDL_GetError() << endl;
    } else {
        if (IMG_Init(IMG_INIT_PNG) < 0) {
            cerr << "SDL_image could not initialise! " << IMG_GetError() << endl;
        } else {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
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
                        cout << "Error initializing GLEW! " << glewGetErrorString(glewError) << endl;
                    }
                    if (SDL_GL_SetSwapInterval(1) < 0) {
                        cout << "Warning: Unable to set VSync! SDL Error: %s\n" << SDL_GetError() << endl;
                    }
                    SDL_SetRelativeMouseMode(SDL_TRUE);

					glEnable(GL_DEPTH_TEST);
					glEnable(GL_TEXTURE_2D);
					//glEnable(GL_POLYGON_SMOOTH);
					glDepthFunc(GL_LESS);

					glGenFramebuffers(1, &depthMapFBO);
					glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

					glGenTextures(1, &depthMap);
					glBindTexture(GL_TEXTURE_2D, depthMap);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT,
						0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
					glBindTexture(GL_TEXTURE_2D, 0);

					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);

					glDrawBuffer(GL_NONE);
					
					if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
						cout << "FAILED\n";
					}

					glGenVertexArrays(1, &vao);
					glBindVertexArray(vao);
					glGenBuffers(1, &quadVertexBuffer);
					glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
					glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
					glBindVertexArray(0);

					quadShader = Shader(util::joinPath({ "resources", "quadvert.glsl" }),
						util::joinPath({ "resources", "quadfrag.glsl" }), true, false, false, false, false, false);
					quadShader.init();

                    glClearColor(0.4f, 0.4f, 0.8f, 1.0f);
                    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                    int width = Chunk::w;
                    int height = Chunk::h;
                    chunks.reserve(10);
                    FastNoise noise;
                    noise.SetNoiseType(FastNoise::SimplexFractal);
					for (int chunkY = 1; chunkY <= 10; ++chunkY) {
						std::vector<Chunk> chunkRow;
						chunkRow.reserve(10);
						for (int chunkX = 1; chunkX <= 10; ++chunkX) {
							Chunk chunk = Chunk(chunkX, chunkY);
							chunk.init();
							for (int z = 0; z < Chunk::l; ++z) {
								for (int x = 0; x < Chunk::w; ++x) {
									int nx = x + ((chunkX - 1) * width);
									int ny = z + ((chunkY - 1) * height);
									float e = 1     * getNoise(noise, nx, ny)
										    + 0.5f  * getNoise(noise, 2 * nx, 2 * ny)
										    + 0.25f * getNoise(noise, 4 * nx, 4 * nx);
									//e = pow(e, 0.4f);
									e = round(e * 24.0f) / 24.0f;
									int randHeight = static_cast<int>(floor(e * 20.0f));
									if (randHeight < 0) {
										randHeight = 1;
									}
									randHeight = min(randHeight, 20);

									for (int h = 0; h < randHeight; ++h) {
										chunk.addCube(x, h, z);
									}
								}
							}
							chunk.generateData();
							chunkRow.push_back(chunk);
						}
						chunks.push_back(chunkRow);
					}
                    for (int chunkY = 0; chunkY < 10; ++chunkY) {
                        for (int chunkX = 0; chunkX < 10; ++chunkX) {
                            shared_ptr<Chunk> left = chunkX > 0 ? make_shared<Chunk>(chunks[chunkY][chunkX - 1]) : nullptr;
                            shared_ptr<Chunk> right = chunkX + 1 < 10 ? make_shared<Chunk>(chunks[chunkY][chunkX + 1]) : nullptr;
                            shared_ptr<Chunk> back = chunkY > 0 ? make_shared<Chunk>(chunks[chunkY - 1][chunkX]) : nullptr;
                            shared_ptr<Chunk> forward = chunkY + 1 < 10 ? make_shared<Chunk>(chunks[chunkY + 1][chunkX]) : nullptr;
							chunks[chunkY][chunkX].update(left, right, forward, back);
                        }
                    }
                    mouse = make_shared<util::Mouse>();
                    inputManager = make_shared<util::InputManager>();
                    inputManager->init();
                    timer = make_shared<util::Timer>();
                    timer->start();
					cam = make_shared<Camera>(glm::vec3(40.0f, 40.0f, 40.0f),
						glm::vec3(0.0f, 1.0f, 0.0f),
						glm::vec3(0.0f, 0.0f, 0.0f),
						SCREEN_WIDTH, SCREEN_HEIGHT);
					cam->update(0.001f, 0.001f);

					float nearPlane = -1.0f, farPlane = 10000.0f;
					//glm::mat4 lightProjection = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH), 0.0f, static_cast<float>(SCREEN_HEIGHT), nearPlane, farPlane);
					glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, nearPlane, farPlane);
					glm::mat4 lightView = glm::lookAt(glm::vec3(80.0f, 100.0f, 80.0f),
						glm::vec3(80.0f, 0.0f, 80.0f),
						glm::vec3(0.0f, 1.0f, 0.0f));

					glm::mat4 biasMatrix(
						0.5, 0.0, 0.0, 0.0,
						0.0, 0.5, 0.0, 0.0,
						0.0, 0.0, 0.5, 0.0,
						0.5, 0.5, 0.5, 1.0
					);

					lightSpace = biasMatrix * lightProjection * lightView;
                }
            }
        }
    }
}

void Game::render() {
    timer->start();
    static glm::mat4 trans = glm::mat4(1.0f);
    const auto &[_posX, _posY] = cam->getChunkPos();
    int posX = _posX / (Chunk::w * 2);
	int posY = _posY / (Chunk::l * 2);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		for (auto& row : chunks) {
			for (auto& c : row) {
				const auto &[chunkX, chunkY] = c.getPos();
				if (abs(chunkX - posX) <= 3 && abs(chunkY - posY) <= 3) {
					c.drawShadow(trans, lightSpace);
				}
			}
		}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	for (auto& row : chunks) {
		for (auto& c : row) {
			const auto &[chunkX, chunkY] = c.getPos();
			if (abs(chunkX - posX) <= 3 && abs(chunkY - posY) <= 3) {
				c.draw(trans, lightSpace, depthMap);
			}
		}
	}
	glViewport(0, 0, 100, 100);
	quadShader.activate(glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f));
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glUniform1i(glGetUniformLocation(quadShader.getProgram(), "tex"), 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	quadShader.deactivate();

    SDL_GL_SwapWindow(window);
    deltaTime = timer->deltaTime();

	float nearPlane = -1.0f, farPlane = 10000.0f;
	//glm::mat4 lightProjection = glm::ortho(-static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_WIDTH), -static_cast<float>(SCREEN_HEIGHT), static_cast<float>(SCREEN_HEIGHT), nearPlane, farPlane);
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 100.0f, nearPlane, farPlane);
	glm::mat4 lightView = glm::lookAt(glm::vec3(80.0f, 100.0f, 80.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

	lightSpace = biasMatrix * lightProjection * lightView;
}

void Game::update() {
    mouse->update();
    cam->update(0, 0);
}

void Game::loop() {
    bool quit = false;
    SDL_Event e;
    bool first = true;;
    while (!quit) {
        render();
        update();
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEMOTION:
                    if (!first) {
                        cam->update(e.motion.xrel, e.motion.yrel);
                    }
                    break;
            }
        }
        if (inputManager->isKeyDown(SDL_SCANCODE_Q)) {
            break;
        }
        first = false;
    }
}

void Game::finalise() {
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;        
    }
    for (auto& row : chunks) {
        for (auto& c : row) {
            c.finalise();
        }
    }
    IMG_Quit();
    SDL_Quit();
}
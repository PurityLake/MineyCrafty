#include <iostream>

#ifdef _WIN32
# include <Windows.h>
#endif

#include <game.hpp>
#include <cube.hpp>
#include <util/util.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/normal.hpp>

using namespace std;
namespace mc = MineyCrafty;


void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	fprintf(stdout, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#ifdef _DEBUG
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif
#else
int main(int argc, char **args) {
#endif

    mc::Game game;
    game.init();
#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
#endif
    game.loop();
    game.finalise();
#if defined(_WIN32) && defined(_DEBUG)
	FreeConsole();
#endif
    return 0;
}
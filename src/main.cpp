#include <iostream>

#ifdef _WIN32
# include <Windows.h>
#endif

#include <game.hpp>
#include <util/util.hpp>

using namespace std;
namespace mc = MineyCrafty;

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
    game.loop();
    game.finalise();
#if defined(_WIN32) && defined(_DEBUG)
	FreeConsole();
#endif
    return 0;
}
#include <iostream>

#ifdef _WIN32
# include <Windows.h>
#endif

#include <game.hpp>
#include <util.hpp>

using namespace std;
namespace mc = MineyCrafty;

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#else
int main(int argc, char **args) {
#endif
    mc::Game game;
    game.init();
    game.loop();
    game.finalise();
    return 0;
}
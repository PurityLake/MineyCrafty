#include <util/timer.hpp>

using namespace std;
using namespace MineyCrafty;
using namespace util;

Timer::Timer() { }
Timer::~Timer() { }

void Timer::start() {
    now  = SDL_GetTicks();
    last = SDL_GetTicks();
}

float Timer::deltaTime() {
    now = SDL_GetTicks();
    return (float)(now - last) / 1000.0;
}
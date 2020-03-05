#include <timer.hpp>

using namespace std;
using namespace MineyCrafty;
using namespace util;

Timer::Timer() { }
Timer::~Timer() { }

void Timer::start() {
    now  = SDL_GetPerformanceCounter();
    last = SDL_GetPerformanceCounter();
}

double Timer::deltaTime() {
    now = SDL_GetPerformanceCounter();
    return (double)((now - last) * 1000 / (double)SDL_GetPerformanceFrequency());
}
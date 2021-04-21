#include <util/timer.hpp>

using namespace std;
using namespace MineyCrafty;
using namespace util;

Timer::Timer() { }
Timer::~Timer() { }

void Timer::start()
{
    now  = SDL_GetTicks();
    last = SDL_GetTicks();
    if (!timer)
    {
        timer = shared_from_this();
    }
}

float Timer::deltaTime()
{
    now = SDL_GetTicks();
    return (float)(now - last) / 1000.0f;
}

std::shared_ptr<Timer> Timer::getTimer()
{
    return timer;
}
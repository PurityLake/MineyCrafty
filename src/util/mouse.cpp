#include <util/mouse.hpp>

using namespace std;
using namespace MineyCrafty;
using namespace util;

Mouse::Mouse() {
    SDL_GetMouseState(&x, &y);
    lastX = x;
    lastY = y;
}

Mouse::~Mouse() { }

void Mouse::update() {
    lastX = x;
    lastY = y;
    SDL_GetMouseState(&x, &y);
    if (!mouse) {
        mouse = shared_from_this();
    }
}

std::pair<int, int> Mouse::getPos() {
    return pair{x, y};
}

std::pair<int, int> Mouse::getLastPos() {
    return pair{lastX, lastY};
}
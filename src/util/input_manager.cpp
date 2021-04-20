#include <util/input_manager.hpp>

using namespace std;
using namespace MineyCrafty;
using namespace util;

InputManager::InputManager()
{
    buttonsDown = SDL_GetKeyboardState(NULL);
}

InputManager::~InputManager()
{

}

void InputManager::init()
{
    if (!inputManager)
    {
        inputManager = shared_from_this();
    }
}

bool InputManager::isKeyDown(char key)
{
    return buttonsDown[key];
}
bool InputManager::isKeyUp(char key)
{
    return !buttonsDown[key];
}

std::shared_ptr<InputManager> InputManager::getInputManager()
{
    return inputManager;
}
#ifndef __HPP_INPUT__
#define __HPP_INPUT__

#include <array>
#include <memory>

#include <SDL.h>

namespace MineyCrafty {
namespace util {

class InputManager : public std::enable_shared_from_this<InputManager> {
private:
    inline static std::shared_ptr<InputManager> inputManager;
    const Uint8 *buttonsDown;

public:
    InputManager();
    virtual ~InputManager();

    bool isKeyDown(char key);
    bool isKeyUp(char key);

    static std::shared_ptr<InputManager> getInputManager();
};

} // namespace util
} // naemspace MineyCrafty

#endif /* __HPP_INPUT__ */
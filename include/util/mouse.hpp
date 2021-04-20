#ifndef __HPP_MOUSE__
#define __HPP_MOUSE__

#include <memory>

#include <SDL.h>

namespace MineyCrafty
{
namespace util
{
    
class Mouse : public std::enable_shared_from_this<Mouse>
{
private:
    static inline std::shared_ptr<Mouse> mouse = nullptr;
    int x, y;
    int lastX, lastY;

public:
    Mouse();
    virtual ~Mouse();

    static std::shared_ptr<Mouse> getMouse();

    void update();

    std::pair<int, int> getPos();
    std::pair<int, int> getLastPos();
};

} // namespace util
} // naemspace MineyCrafty

#endif /* __HPP_MOUSE__ */
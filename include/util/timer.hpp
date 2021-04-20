#ifndef __HPP_TIMER__
#define __HPP_TIMER__

#include <iostream>
#include <memory>

#include <SDL.h>

namespace MineyCrafty
{
namespace util
{

class Timer : public std::enable_shared_from_this<Timer>
{
private:
    uint64_t now, last;
    static inline std::shared_ptr<Timer> timer = nullptr;

public:
    Timer();
    virtual ~Timer();
    
    void start();
    float deltaTime(); 

    static std::shared_ptr<Timer> getTimer();
};

} // namespace util
} // namespace MineyCrafty

#endif /* __HPP_TIMER__ */
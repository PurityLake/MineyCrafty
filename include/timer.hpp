#ifndef __HPP_TIMER__
#define __HPP_TIMER__

#include <iostream>

#include <SDL.h>

namespace MineyCrafty {
namespace util {

class Timer {
private:
    uint64_t now, last;

public:
    Timer();
    virtual ~Timer();
    
    void start();
    double deltaTime(); 
};

} // namespace util
} // namespace MineyCrafty

#endif /* __HPP_TIMER__ */
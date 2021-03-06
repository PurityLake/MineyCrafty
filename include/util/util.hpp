#ifndef __HPP_UTIL__
#define __HPP_UTIL__

#include <iostream>
#include <sstream>
#include <string>

#include <util/input_manager.hpp>
#include <util/mouse.hpp>
#include <util/timer.hpp>

namespace MineyCrafty
{
namespace util
{

inline std::string joinPath(std::initializer_list<std::string> paths)
{
#ifdef _WIN32
    static const char pathSep = '\\';
#else
    static const char pathSep = '/';
#endif
    size_t i = 0;
    std::stringstream ss;
    ss << '.' << pathSep;
    for (const auto& p : paths)
    {
        ss << p;
        if (i + 1 < paths.size())
        {
            ss << pathSep;
        }
        ++i;
    }
    return ss.str();
};

} // namesapce util
} // namespace MineyCrafty

#endif /* __HPP_UTIL__ */
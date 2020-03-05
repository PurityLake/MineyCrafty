#ifndef __HPP_NOISE__
#define __HPP_NOISE__

#include <vector>

namespace MineyCrafty {
namespace util {

class PerlinNoise {
private:
    std::vector<int> p;

    double fade(double t);
    double lerp(double t, double a, double b);
    double grad(int hash, double x, double y, double z);

public:
    PerlinNoise();
    PerlinNoise(unsigned int seed);

    double noise(double x, double y, double z);
};

class NoiseVector {
private:
    PerlinNoise pn;
    std::vector<double> noise;
    int width, height;

public:
    NoiseVector();
    NoiseVector(int w, int h);
    NoiseVector(int w, int h, unsigned int seed);

    double getAt(int x, int y);
    void generate();
};

} // namespace util
} // namespace MineyCrafty

#endif /* __HPP_NOISE__ */
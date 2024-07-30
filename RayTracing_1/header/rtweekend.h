#pragma once
#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <functional>
#include <random>


// Usings

using std::shared_ptr;
using std::make_shared;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) 
{
    return degrees * pi / 180;
}

inline double ffmin(double a, double b) { return a <= b ? a : b; }
inline double ffmax(double a, double b) { return a >= b ? a : b; }

//随机数
inline double random_double(double min, double max)
{
    static std::uniform_real_distribution<double> distribution(min, max);   //生成[min,max]范围内的浮点数
    static std::mt19937 generator;  //使用Mersenne Twister引擎，生成高质量的伪随机数。
    static std::function<double()> rand_generator = std::bind(distribution, generator);
    return rand_generator();
}

inline double random_double()
{
    return random_double(0, 1);
}

inline int random_int(int min, int max)
{
    static std::uniform_int_distribution<int> distribution(min, max);   //生成[min,max]范围内的整数
    static std::mt19937 generator;  //使用Mersenne Twister引擎，生成高质量的伪随机数。
    static std::function<int()> rand_generator = std::bind(distribution, generator);
    return rand_generator();
}

inline int random_int()
{
    return random_int(0, 100);
}

//限制x
inline double clamp(double x, double min, double max) 
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// Common Headers

#include "ray.h"
#include "vec3.h"
#pragma once
#include "rtweekend.h"
#include "Perlin.h"

class texture 
{
public:
    virtual Vec3 value(double u, double v, const Vec3& p) const = 0;
};

class constant_texture : public texture 
{
public:
    constant_texture() {}
    constant_texture(Vec3 c) : color(c) {}

    virtual Vec3 value(double u, double v, const Vec3& p) const 
    {
        return color;
    }

public:
    Vec3 color;
};

//棋盘纹理
class checker_texture : public texture 
{
public:
    checker_texture() {}
    checker_texture(shared_ptr<texture> t0, shared_ptr<texture> t1) : even(t0), odd(t1) {}

    virtual Vec3 value(double u, double v, const Vec3& p) const 
    {
        auto sines = sin(10 * p.X()) * sin(10 * p.Y()) * sin(10 * p.Z());
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }

public:
    shared_ptr<texture> odd;
    shared_ptr<texture> even;
};


//柏林噪声
class noise_texture : public texture 
{
public:
    noise_texture() : scale(1.0) {}
    noise_texture(double sc) : scale(sc) {}

    virtual Vec3 value(double u, double v, const Vec3& p) const 
    {
        //柏林插值的输出结果有可能是负数, 这些负数在伽马校正时经过开平方跟`sqrt()`会变成NaN。我们将输出结果映射到0与1之间。
        return Vec3(1, 1, 1) * 0.5 * (1.0 + sin(scale * p.Z() + 10 * noise.turb(p)));
        //因为p的范围是[0,255]，但是取像素时的坐标超过255，会重复柏林噪声
        //增大坐标，相当于加快采样频率
    }

public:
    perlin noise;
    double scale;
};
#pragma once
#include "Texture.h"

class image_texture : public texture 
{
public:
    image_texture() : data(0), nx(0), ny(0) {}
    image_texture(unsigned char* pixels, int A, int B)
        : data(pixels), nx(A), ny(B) {}

    ~image_texture() 
    {
        delete data;
    }

    virtual Vec3 value(double u, double v, const Vec3& p) const 
    {
        // If we have no texture data, then always emit cyan (as a debugging aid).
        if (data == nullptr)
            return Vec3(0, 1, 1);

        auto i = static_cast<int>((1 - u) * nx);
        auto j = static_cast<int>((1 - v) * ny - 0.001);

        if (i < 0) i = 0;
        if (j < 0) j = 0;
        if (i > nx - 1) i = nx - 1;
        if (j > ny - 1) j = ny - 1;

        auto r = static_cast<int>(data[3 * i + 3 * nx * j + 0]) / 255.0;
        auto g = static_cast<int>(data[3 * i + 3 * nx * j + 1]) / 255.0;
        auto b = static_cast<int>(data[3 * i + 3 * nx * j + 2]) / 255.0;

        return Vec3(r, g, b);
    }

public:
    unsigned char* data;
    int nx, ny;     //ÏñËØ
};
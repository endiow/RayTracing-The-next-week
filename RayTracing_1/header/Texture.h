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

//��������
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


//��������
class noise_texture : public texture 
{
public:
    noise_texture() : scale(1.0) {}
    noise_texture(double sc) : scale(sc) {}

    virtual Vec3 value(double u, double v, const Vec3& p) const 
    {
        //���ֲ�ֵ���������п����Ǹ���, ��Щ������٤��У��ʱ������ƽ����`sqrt()`����NaN�����ǽ�������ӳ�䵽0��1֮�䡣
        return Vec3(1, 1, 1) * 0.5 * (1.0 + sin(scale * p.Z() + 10 * noise.turb(p)));
        //��Ϊp�ķ�Χ��[0,255]������ȡ����ʱ�����곬��255�����ظ���������
        //�������꣬�൱�ڼӿ����Ƶ��
    }

public:
    perlin noise;
    double scale;
};
#pragma once
#include "rtweekend.h"

class aabb 
{
    Vec3 _min;
    Vec3 _max;

public:
    aabb() {}
    aabb(const Vec3& a, const Vec3& b) { _min = a; _max = b; }  //左下角和右上角

    Vec3 min() const { return _min; }
    Vec3 max() const { return _max; }

    inline bool Hit_box(const Ray& r, double tmin, double tmax) const;

    //包围盒的包围盒
    static aabb surrounding_box(aabb box0, aabb box1);
};

inline bool aabb::Hit_box(const Ray& r, double tmin, double tmax) const
{
    for (int a = 0; a < 3; a++)
    {
        auto invD = 1.0f / r.Direction()[a];
        auto t0 = (min()[a] - r.Origin()[a]) * invD;
        auto t1 = (max()[a] - r.Origin()[a]) * invD;
        //当且仅当视线的方向向量在当前计算维度的分量中为负，此时 t0 > t1
        if (invD < 0.0f)
            std::swap(t0, t1);
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;
        if (tmax <= tmin)
            return false;
    }
    return true;
}

aabb aabb::surrounding_box(aabb box0, aabb box1) 
{
    Vec3 small(ffmin(box0.min().X(), box1.min().X()),
        ffmin(box0.min().Y(), box1.min().Y()),
        ffmin(box0.min().Z(), box1.min().Z()));

    Vec3 big(ffmax(box0.max().X(), box1.max().X()),
        ffmax(box0.max().Y(), box1.max().Y()),
        ffmax(box0.max().Z(), box1.max().Z()));

    return aabb(small, big);
}
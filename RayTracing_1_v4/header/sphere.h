#pragma once

#include "hittable.h"
#include "rtweekend.h"

class sphere : public hittable 
{
public:
    // Stationary Sphere
    sphere(const point3& center, double radius, shared_ptr<material> mat)
        : center1(center), radius(std::fmax(0, radius)), mat(mat), is_moving(false)
    {
        auto rvec = vec3(radius, radius, radius);
        bbox = aabb(center1 - rvec, center1 + rvec);
    }

    // Moving Sphere
    sphere(const point3& center1, const point3& center2, double radius, shared_ptr<material> mat)
        : center1(center1), radius(std::fmax(0, radius)), mat(mat), is_moving(true)
    {
        auto rvec = vec3(radius, radius, radius);
        aabb box1(center1 - rvec, center1 + rvec);
        aabb box2(center2 - rvec, center2 + rvec);
        bbox = aabb(box1, box2);

        center_vec = center2 - center1;
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override 
    {
        point3 center = is_moving ? sphere_center(r.time()) : center1;
        vec3 oc = center - r.origin();
        auto a = r.direction().length_squared();
        auto b = dot(r.direction(), oc);
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = b * b - a * c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (b - sqrtd) / a;    //�����ʽ
        if (!ray_t.surrounds(root))
        {
            root = (b + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        get_sphere_uv(outward_normal, rec.u, rec.v);
        rec.mat = mat;

        return true;
    }

    aabb bounding_box() const override { return bbox; }

private:
    point3 center1;
    double radius;
    shared_ptr<material> mat;
    bool is_moving;
    vec3 center_vec;
    aabb bbox;

    point3 sphere_center(double time) const 
    {
        // Linearly interpolate from center1 to center2 according to time, where t=0 yields
        // center1, and t=1 yields center2.
        return center1 + time * center_vec;
    }

    static void get_sphere_uv(const point3& p, double& u, double& v) 
    {
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

        auto theta = std::acos(-p.y());
        auto phi = std::atan2(-p.z(), p.x()) + pi;

        u = phi / (2 * pi);
        v = theta / pi;
    }
};
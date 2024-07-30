#pragma once
#include <algorithm>
#include "HitableList.h"

class bvh_node : public Hitable 
{
    shared_ptr<Hitable> left;
    shared_ptr<Hitable> right;
    aabb box;

public:
    bvh_node() {}

    bvh_node(std::vector<shared_ptr<Hitable>>& objects, size_t start, size_t end, double time0, double time1);

    bvh_node(HitableList& list, double time0, double time1)
        : bvh_node(list.objects, 0, list.Obj().size(), time0, time1)
    {}

    virtual bool Hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const;
    virtual bool bounding_box(double t0, double t1, aabb& output_box) const;
};

inline bool box_compare(const shared_ptr<Hitable> a, const shared_ptr<Hitable> b, int axis) 
{
    aabb box_a;
    aabb box_b;

    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
        std::cerr << "No bounding box in bvh_node constructor.\n";

    return box_a.min().E[axis] < box_b.min().E[axis];
}

bool box_x_compare(const shared_ptr<Hitable> a, const shared_ptr<Hitable> b) 
{
    return box_compare(a, b, 0);
}

bool box_y_compare(const shared_ptr<Hitable> a, const shared_ptr<Hitable> b) 
{
    return box_compare(a, b, 1);
}

bool box_z_compare(const shared_ptr<Hitable> a, const shared_ptr<Hitable> b) 
{
    return box_compare(a, b, 2);
}

bvh_node::bvh_node(std::vector<shared_ptr<Hitable>>& objects, size_t start, size_t end, double time0, double time1)
{
    int axis = random_int(0, 2);
    auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare : box_z_compare;

    size_t object_span = end - start;   //物体个数

    if (object_span == 1) 
    {
        left = right = objects[start];
    }
    else if (object_span == 2) 
    {
        if (comparator(objects[start], objects[start + 1]))     //比较物体谁的包围盒在左边
        {
            left = objects[start];      //坐标值小的为左子树
            right = objects[start + 1];
        }
        else 
        {
            left = objects[start + 1];
            right = objects[start];
        }
    }
    else 
    {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + object_span / 2;
        left = make_shared<bvh_node>(objects, start, mid, time0, time1);    //递归生成bvh树
        right = make_shared<bvh_node>(objects, mid, end, time0, time1);
    }

    aabb box_left, box_right;

    //检查物体是否有包围盒 如无限延伸的平面
    if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right))
        std::cerr << "No bounding box in bvh_node constructor.\n";

    box = aabb::surrounding_box(box_left, box_right);   //归并左右子树
}

bool bvh_node::Hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const 
{
    if (!box.Hit_box(r, t_min, t_max))
        return false;

    //检测包围盒的子节点是否击中
    bool hit_left = left->Hit(r, t_min, t_max, rec);
    bool hit_right = right->Hit(r, t_min, hit_left ? rec.T : t_max, rec);

    return hit_left || hit_right;
}

bool bvh_node::bounding_box(double t0, double t1, aabb& output_box) const 
{
    output_box = box;
    return true;
}
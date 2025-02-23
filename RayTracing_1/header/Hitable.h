#pragma once

#include "Ray.h"
#include "rtweekend.h"
#include "AABB.h"

class Material;

struct HitRecord   //命中结果
{
	double T;		//入射点解出来的T
	Vec3 P;			//入射点
	Vec3 Normal;	//法线
	bool front_face;//判断法线方向
	shared_ptr<Material> mat_ptr;	//材质
	double u;	//纹理
	double v;

	inline void set_face_normal(const Ray& r, const Vec3& outward_normal) 
	{
		front_face = Dot(r.Direction(), outward_normal) < 0;
		Normal = front_face ? outward_normal : -outward_normal;//法线总是朝外
	}
};

class Hitable
{
public:
	//TMin与TMax分别表示检测射线的开始与结束点对应地系数T
	//也就是检测射线为R = P(TMin)到R=P(TMax）之间得这段射线
	virtual bool Hit(const Ray& R, double TMin, double TMax, HitRecord& Rec) const = 0;//纯虚函数

	virtual bool bounding_box(double t0, double t1, aabb& output_box) const = 0;
};
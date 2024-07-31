#pragma once

#include "Hitable.h"
#include "AABB.h"

class Sphere : public Hitable
{
	Vec3 Center = { 0,0,0 }; //球心
	double Radius = 0;//半径
	shared_ptr<Material> mat_ptr;

public:
	Sphere() {}
	Sphere(Vec3 Cen, double R, shared_ptr<Material> m) :Center(Cen), Radius(R), mat_ptr(m) {};	//构造函数，初始化球心与半径

	inline virtual bool Hit(const Ray& R, double TMin, double TMax, HitRecord& Rec) const;	//继承Hitable的Hit

	bool bounding_box(double t0, double t1, aabb& output_box) const;	//计算包围盒

	void get_sphere_uv(const Vec3& p, double& u, double& v) const;	//算出球体u,v坐标
};


//如果射线R在Tmin与Tmax范围内命中当前球体，则返回true
bool Sphere::Hit(const Ray& R, double TMin, double TMax, HitRecord& Rec)const
{
	//解一元二次方程
	Vec3 OC = R.Origin() - Center;//从球心到射线发射点的向量
	double A = Dot(R.Direction(), R.Direction());
	double B = Dot(OC, R.Direction());
	double C = Dot(OC, OC) - Radius * Radius;
	double Discriminant = B * B - A * C;

	if (Discriminant > 0) //如果有两个解
	{
		//Rec.MatPtr = MatPtr;
		//计算比较小的解
		double Temp = (-B - sqrt(B * B - A * C)) / A;
		if (Temp < TMax && Temp >TMin) //这个点解出来的T在TMin与TMax之间，是我们想要的值
		{
			//保存入射点的解、入射点位置和入射点法线到Rec中
			Rec.T = Temp;
			Rec.P = R.PointAtParameter(Rec.T);
			Vec3 outward_normal = (Rec.P - Center) / Radius;
			Rec.set_face_normal(R, outward_normal);		//设置法线
			Rec.mat_ptr = mat_ptr;
			get_sphere_uv((Rec.P - Center) / Radius, Rec.u, Rec.v);		//获取纹理坐标
			return true;
		}
		//如果上边的点不在我们的检测范围内，则再去用同样的方法检测另一个解
		Temp = (-B + sqrt(B * B - A * C)) / A;
		if (Temp < TMax && Temp >TMin)
		{
			//保存入射点的解、入射点位置和入射点法线到Rec中
			Rec.T = Temp;
			Rec.P = R.PointAtParameter(Rec.T);
			Vec3 outward_normal = (Rec.P - Center) / Radius;
			Rec.set_face_normal(R, outward_normal);
			Rec.mat_ptr = mat_ptr;
			get_sphere_uv((Rec.P - Center) / Radius, Rec.u, Rec.v);
			return true;
		}
	}
	//如果射线没命中球体则返回false
	return false;
}

bool Sphere::bounding_box(double t0, double t1, aabb& output_box) const 
{
	output_box = aabb(Center - Vec3(Radius, Radius, Radius), Center + Vec3(Radius, Radius, Radius));
	return true;
}

void Sphere::get_sphere_uv(const Vec3& p, double& u, double& v) const
{
	auto phi = atan2(p.Z(), p.X());
	auto theta = asin(p.Y());
	u = 1 - (phi + pi) / (2 * pi);
	v = (theta + pi / 2) / pi;
}


class Moving_Sphere : public Hitable
{
	Vec3 Center0, Center1; //球心
	double time0 = 0, time1 = 0;
	double Radius = 0;//半径
	shared_ptr<Material> mat_ptr;

public:
	Moving_Sphere() {}
	Moving_Sphere(Vec3 cen0, Vec3 cen1, double t0, double t1, double r, shared_ptr<Material> m)
		: Center0(cen0), Center1(cen1), time0(t0), time1(t1), Radius(r), mat_ptr(m) {};	//构造函数，初始化球心与半径

	inline virtual bool Hit(const Ray& R, double TMin, double TMax, HitRecord& Rec) const;	//继承Hitable的Hit

	Vec3 Center(double time) const;

	bool bounding_box(double t0, double t1, aabb& output_box) const;
};

//如果射线R在Tmin与Tmax范围内命中当前球体，则返回true
bool Moving_Sphere::Hit(const Ray& R, double TMin, double TMax, HitRecord& Rec) const
{
	//解一元二次方程
	Vec3 OC = R.Origin() - Center(R.time());//从球心到射线发射点的向量
	double A = Dot(R.Direction(), R.Direction());
	double B = Dot(OC, R.Direction());
	double C = Dot(OC, OC) - Radius * Radius;
	double Discriminant = B * B - A * C;

	if (Discriminant > 0) //如果有两个解
	{

		//Rec.MatPtr = MatPtr;
		//计算比较小的解
		double Temp = (-B - sqrt(B * B - A * C)) / A;
		if (Temp < TMax && Temp >TMin) //这个点解出来的T在TMin与TMax之间，是我们想要的值
		{
			//保存入射点的解、入射点位置和入射点法线到Rec中
			Rec.T = Temp;
			Rec.P = R.PointAtParameter(Rec.T);
			Vec3 outward_normal = (Rec.P - Center(R.time())) / Radius;
			Rec.set_face_normal(R, outward_normal);
			Rec.mat_ptr = mat_ptr;
			return true;
		}
		//如果上边的点不在我们的检测范围内，则再去用同样的方法检测另一个解
		Temp = (-B + sqrt(B * B - A * C)) / A;
		if (Temp < TMax && Temp >TMin)
		{
			//保存入射点的解、入射点位置和入射点法线到Rec中
			Rec.T = Temp;
			Rec.P = R.PointAtParameter(Rec.T);
			Vec3 outward_normal = (Rec.P - Center(R.time())) / Radius;
			Rec.set_face_normal(R, outward_normal);
			Rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	//如果射线没命中球体则返回false
	return false;
}

Vec3 Moving_Sphere::Center(double time) const 
{
	return Center0 + ((time - time0) / (time1 - time0)) * (Center1 - Center0);	//插值得出这个时间点球的位置
}

bool Moving_Sphere::bounding_box(double t0, double t1, aabb& output_box) const 
{
	aabb box0(
		Center(t0) - Vec3(Radius, Radius, Radius),
		Center(t0) + Vec3(Radius, Radius, Radius));

	aabb box1(
		Center(t1) - Vec3(Radius, Radius, Radius),
		Center(t1) + Vec3(Radius, Radius, Radius));

	output_box = aabb::surrounding_box(box0, box1);
	return true;
}

//矩形
class xy_rect : public Hitable 
{
	shared_ptr<Material> mp;
	double x0, x1, y0, y1, k;

public:
	xy_rect() : x0(), x1(), y0(), y1(), k(), mp() {}

	xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<Material> mat)
		: x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {}

	virtual bool Hit(const Ray& R, double TMin, double TMax, HitRecord& Rec) const
	{
		auto t = (k - R.Origin().Z()) / R.Direction().Z();

		if (t < TMin || t > TMax)
			return false;

		auto x = R.Origin().X() + t * R.Direction().X();
		auto y = R.Origin().Y() + t * R.Direction().Y();

		if (x < x0 || x > x1 || y < y0 || y > y1)
			return false;

		Rec.u = (x - x0) / (x1 - x0);
		Rec.v = (y - y0) / (y1 - y0);
		Rec.T = t;

		Vec3 outward_normal = Vec3(0, 0, 1);
		Rec.set_face_normal(R, outward_normal);
		//rec.Normal = Vec3(0, 0, -1);
		Rec.mat_ptr = mp;
		Rec.P = R.PointAtParameter(t);

		return true;
	}

	virtual bool bounding_box(double t0, double t1, aabb& output_box) const 
	{
		output_box = aabb(Vec3(x0, y0, k - 0.0001), Vec3(x1, y1, k + 0.0001));
		return true;
	}
};

class xz_rect : public Hitable
{
	shared_ptr<Material> mp;
	double x0, x1, z0, z1, k;

public:
	xz_rect() : x0(), x1(), z0(), z1(), k(), mp() {}

	xz_rect(double _x0, double _x1, double _z0, double _z1, double _k, shared_ptr<Material> mat)
		: x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat) {}

	virtual bool Hit(const Ray& R, double TMin, double TMax, HitRecord& Rec) const
	{
		auto t = (k - R.Origin().Y()) / R.Direction().Y();

		if (t < TMin || t > TMax)
			return false;

		auto x = R.Origin().X() + t * R.Direction().X();
		auto z = R.Origin().Z() + t * R.Direction().Z();

		if (x < x0 || x > x1 || z < z0 || z > z1)
			return false;

		Rec.u = (x - x0) / (x1 - x0);
		Rec.v = (z - z0) / (z1 - z0);
		Rec.T = t;

		Vec3 outward_normal = Vec3(0, 1, 0);
		Rec.set_face_normal(R, outward_normal);
		//Rec.Normal = Vec3(0, 1, 0);
		Rec.mat_ptr = mp;
		Rec.P = R.PointAtParameter(t);

		return true;
	}

	virtual bool bounding_box(double t0, double t1, aabb& output_box) const
	{
		output_box = aabb(Vec3(x0, k - 0.0001, z0), Vec3(x1, k + 0.0001, z1));
		return true;
	}
};

class yz_rect : public Hitable
{
	shared_ptr<Material> mp;
	double y0, y1, z0, z1, k;

public:
	yz_rect() : y0(), y1(), z0(), z1(), k(), mp() {}

	yz_rect(double _y0, double _y1, double _z0, double _z1, double _k, shared_ptr<Material> mat)
		: y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {}

	virtual bool Hit(const Ray& R, double TMin, double TMax, HitRecord& Rec) const
	{
		auto t = (k - R.Origin().X()) / R.Direction().X();
		if (t < TMin || t > TMax)
			return false;
		auto y = R.Origin().Y() + t * R.Direction().Y();
		auto z = R.Origin().Z() + t * R.Direction().Z();
		if (y < y0 || y > y1 || z < z0 || z > z1)
			return false;
		Rec.u = (y - y0) / (y1 - y0);
		Rec.v = (z - z0) / (z1 - z0);
		Rec.T = t;
		Vec3 outward_normal = Vec3(1, 0, 0);
		Rec.set_face_normal(R, outward_normal);
		//Rec.Normal = Vec3(1, 0, 0);
		Rec.mat_ptr = mp;
		Rec.P = R.PointAtParameter(t);
		return true;
	}

	virtual bool bounding_box(double t0, double t1, aabb& output_box) const
	{
		output_box = aabb(Vec3(k - 0.0001, y0, z0), Vec3(k + 0.0001, y1, z1));
		return true;
	}
};


//翻转矩形
class flip_face : public Hitable 
{
	shared_ptr<Hitable> ptr;

public:
	flip_face(shared_ptr<Hitable> p) : ptr(p) {}

	virtual bool Hit(const Ray& R, double TMin, double TMax, HitRecord& Rec) const 
	{
		if (!ptr->Hit(R, TMin, TMax, Rec))	//没有击中
			return false;

		Rec.front_face = !Rec.front_face;
		return true;
	}

	virtual bool bounding_box(double t0, double t1, aabb& output_box) const 
	{
		return ptr->bounding_box(t0, t1, output_box);
	}
};
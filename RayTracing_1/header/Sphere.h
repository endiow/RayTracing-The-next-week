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
			Vec3 outward_normal = (Rec.P - Center) / Radius;
			Rec.set_face_normal(R, outward_normal);
			Rec.mat_ptr = mat_ptr;
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